#ifndef DEPTH_TO_TRIPLE_H_Y021ENVZ
#define DEPTH_TO_TRIPLE_H_Y021ENVZ

#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <limits>
#include <opencv2/core/mat.hpp>
#include <sens_loc/camera_models/pinhole.h>
#include <sens_loc/conversion/util.h>
#include <sens_loc/math/eigen_types.h>
#include <taskflow/taskflow.hpp>

namespace sens_loc { namespace conversion {

/// Convert range image to a flexion-image.
///
/// The flexion image is a derived image type that describes the angle between
/// the normals of the surface patch at pixel (u,v). The normals are estimated
/// with the direct neighbours and and the diagonal neighbours.
/// This is a measure of curvature as well.
///
/// \tparam Real precision of the calculation
/// \tparam PixelType underlying type of \p depth_image
/// \param depth_image range image
/// \param intrinsic calibration of the sensor that took the image
/// \returns flexion image, each pixel in the range \f$[0,1]\f$
/// \sa conversion::depth_to_laserscan
/// \pre \p depth_image is not empty
/// \pre intrinsic matches the sensor that took the image
template <typename Real = float, typename PixelType = float>
cv::Mat
depth_to_flexion(const cv::Mat &                     depth_image,
                 const camera_models::pinhole<Real> &intrinsic) noexcept;

/// Convert range image to a flexion image in parallel
//
/// This function implements the same functionality but with row-parallelism.
//
/// \sa depth_to_flexion
/// \param[in] depth_image,intrinsic same as in \p depth_to_flexion
/// \param[out] triple_image output image
/// \param[inout] flow taskgraph the calculations will be registered in
/// \returns synchronization task before and after the calculation
/// \note the calculation does not happen instantly but first a taskgraph is
/// \pre \p triple_image has the same dimension as \p depth_image
/// \pre the underlying types match the defined template parameters
/// built. This graph will then execute all the tasks on request.
template <typename Real = float, typename PixelType = float>
std::pair<tf::Task, tf::Task>
par_depth_to_flexion(const cv::Mat &                     depth_image,
                     const camera_models::pinhole<Real> &intrinsic,
                     cv::Mat &triple_image, tf::Taskflow &flow) noexcept;

/// Scale the flexion image to \p PixelType for normal image visualization.
///
/// This function simply scales the image to the full possible range of
/// \p PixelType.
/// \sa cv::Mat::convertTo
/// \pre underlying types match
/// \pre range of each pixel is \f$[0, 1]\f$
/// \post range of result pixels is \f$[PixelType_{min}, PixelType_{max}]\f$
template <typename Real, typename PixelType>
cv::Mat convert_flexion(const cv::Mat &flexion_image) noexcept;


namespace detail {
using ::sens_loc::math::vec;
template <typename Real = float>
math::camera_coord<Real>
to_camera(const camera_models::pinhole<Real> &intrinsic,
          const math::pixel_coord<Real> &p, Real d) noexcept {
    const math::sphere_coord<Real> P_s = intrinsic.pixel_to_sphere(p);
    return math::camera_coord<Real>(d * P_s.Xs(), d * P_s.Ys(), d * P_s.Zs());
}

template <typename Real = float, typename PixelType = float>
inline void flexion_inner(int v, const cv::Mat &depth_image,
                          const camera_models::pinhole<Real> &intrinsic,
                          cv::Mat &                           out) {
    for (int u = 1; u < depth_image.cols - 1; ++u) {
        const Real d__1__0 = depth_image.at<PixelType>(v - 1, u);
        const Real d_1__0  = depth_image.at<PixelType>(v + 1, u);

        const Real d__0__1 = depth_image.at<PixelType>(v, u - 1);
        const Real d__0_1  = depth_image.at<PixelType>(v, u + 1);

        const Real d__1_1 = depth_image.at<PixelType>(v - 1, u + 1);
        const Real d_1__1 = depth_image.at<PixelType>(v + 1, u - 1);

        const Real d__1__1 = depth_image.at<PixelType>(v - 1, u - 1);
        const Real d_1_1   = depth_image.at<PixelType>(v + 1, u + 1);

        // If any of the depths is zero, the resulting vector will be the
        // null vector. This with then propagate through as zero and does not
        // induce any undefined behaviour or other problems.
        // Not short-circuiting results in easier vectorization / GPU
        // acceleration.

        using math::camera_coord;
        using math::pixel_coord;

        const camera_coord<Real> surface_pt0 =
            to_camera(intrinsic, pixel_coord<Real>(u, v - 1), d__1__0);
        const camera_coord<Real> surface_pt1 =
            to_camera(intrinsic, pixel_coord<Real>(u, v + 1), d_1__0);
        const camera_coord<Real> surface_dir0 = surface_pt1 - surface_pt0;

        const camera_coord<Real> surface_pt2 =
            to_camera(intrinsic, pixel_coord<Real>(u - 1, v), d__0__1);
        const camera_coord<Real> surface_pt3 =
            to_camera(intrinsic, pixel_coord<Real>(u + 1, v), d__0_1);
        const camera_coord<Real> surface_dir1 = surface_pt3 - surface_pt2;

        const camera_coord<Real> surface_pt4 =
            to_camera(intrinsic, pixel_coord<Real>(u + 1, v - 1), d__1_1);
        const camera_coord<Real> surface_pt5 =
            to_camera(intrinsic, pixel_coord<Real>(u - 1, v + 1), d_1__1);
        const camera_coord<Real> surface_dir2 = surface_pt5 - surface_pt4;

        const camera_coord<Real> surface_pt6 =
            to_camera(intrinsic, pixel_coord<Real>(u - 1, v - 1), d__1__1);
        const camera_coord<Real> surface_pt7 =
            to_camera(intrinsic, pixel_coord<Real>(u + 1, v + 1), d_1_1);
        const camera_coord<Real> surface_dir3 = surface_pt7 - surface_pt6;

        const auto cross0 =
            surface_dir0.normalized().cross(surface_dir1.normalized());
        const auto cross1 =
            surface_dir2.normalized().cross(surface_dir3.normalized());

        const auto triple = std::abs(cross0.dot(cross1));

        Ensures(triple >= 0.);
        Ensures(triple <= 1.);

        out.at<Real>(v, u) = triple;
    }
}

}  // namespace detail

template <typename Real, typename PixelType>
inline cv::Mat
depth_to_flexion(const cv::Mat &                     depth_image,
                 const camera_models::pinhole<Real> &intrinsic) noexcept {
    Expects(depth_image.type() == detail::get_cv_type<PixelType>());
    Expects(depth_image.channels() == 1);
    Expects(!depth_image.empty());
    Expects(depth_image.cols > 2);
    Expects(depth_image.rows > 2);

    cv::Mat triple(depth_image.rows, depth_image.cols,
                   detail::get_cv_type<Real>());
    for (int v = 1; v < depth_image.rows - 1; ++v) {
        detail::flexion_inner<Real, PixelType>(v, depth_image, intrinsic,
                                               triple);
    }

    Ensures(triple.cols == depth_image.cols);
    Ensures(triple.rows == depth_image.rows);
    Ensures(triple.type() == detail::get_cv_type<Real>());
    Ensures(triple.channels() == 1);

    return triple;
}

/// Convert an euclidian depth image to a triple-product-image.
template <typename Real, typename PixelType>
inline std::pair<tf::Task, tf::Task>
par_depth_to_flexion(const cv::Mat &                     depth_image,
                     const camera_models::pinhole<Real> &intrinsic,
                     cv::Mat &triple_image, tf::Taskflow &flow) noexcept {
    Expects(depth_image.type() == detail::get_cv_type<PixelType>());
    Expects(depth_image.channels() == 1);
    Expects(!depth_image.empty());
    Expects(depth_image.cols > 2);
    Expects(depth_image.rows > 2);
    Expects(triple_image.cols == depth_image.cols);
    Expects(triple_image.rows == depth_image.rows);
    Expects(triple_image.channels() == depth_image.channels());
    Expects(triple_image.type() == detail::get_cv_type<Real>());

    auto sync_points = flow.parallel_for(
        1, depth_image.rows - 1, 1, [&](int v) noexcept {
            detail::flexion_inner<Real, PixelType>(v, depth_image, intrinsic,
                                                   triple_image);
        });

    return sync_points;
}

template <typename Real, typename PixelType>
inline cv::Mat convert_flexion(const cv::Mat &flexion_image) noexcept {
    Expects(flexion_image.channels() == 1);
    Expects(flexion_image.rows > 2);
    Expects(flexion_image.cols > 2);
    Expects(flexion_image.type() == detail::get_cv_type<Real>());

    cv::Mat    img(flexion_image.rows, flexion_image.cols,
                detail::get_cv_type<PixelType>());
    const auto scale = std::numeric_limits<PixelType>::max() -
                       std::numeric_limits<PixelType>::min();
    const auto offset = std::numeric_limits<PixelType>::min();

    flexion_image.convertTo(img, detail::get_cv_type<PixelType>(), scale,
                            offset);

    Ensures(img.cols == flexion_image.cols);
    Ensures(img.rows == flexion_image.rows);
    Ensures(img.type() == detail::get_cv_type<PixelType>());
    Ensures(img.channels() == 1);

    return img;
}
}}  // namespace sens_loc::conversion

#endif /* end of include guard: DEPTH_TO_TRIPLE_H_Y021ENVZ */
