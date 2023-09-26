#ifndef DEPTH_TO_TRIPLE_H_Y022ENVZ
#define DEPTH_TO_TRIPLE_H_Y022ENVZ

#include <cmath>
#include <iostream>
#include <limits>
#include <sens_loc/camera_models/pinhole.h>
#include <sens_loc/conversion/util.h>
#include <sens_loc/math/eigen_types.h>
#include <sens_loc/math/image.h>
#include <taskflow/taskflow.hpp>

namespace sens_loc::conversion {

/// Convert range image to a flexion-image.
///
/// The flexion image is a derived image type that describes the angle between
/// the normals of the surface patch at pixel (u,v). The normals are estimated
/// with the direct neighbours and and the diagonal neighbours.
/// This is a measure of curvature as well.
///
/// \tparam Real precision of the calculation, floating-point
/// \tparam Intrinsic camera model that projects pixel to the unit sphere
/// \param depth_image range image
/// \param intrinsic calibration of the sensor that took the image
/// \param neighbors Number of neighboring pixels 
/// \returns flexion image, each pixel in the range \f$[0,1]\f$
/// \sa conversion::depth_to_laserscan
/// \pre \p depth_image is not empty
/// \pre intrinsic matches the sensor that took the image
/// \sa camera_models::is_intrinsic_v
template <template <typename> typename Intrinsic, typename Real = float>
math::image<Real> depth_to_flexion_nxn(const math::image<Real>& depth_image,
                                       const Intrinsic<Real>&   intrinsic,
                                       const int& neighbors) noexcept;

/// Convert range image to a flexion image in parallel
//
/// This function implements the same functionality but with row-parallelism.
//
/// \sa depth_to_flexion
/// \param[in] depth_image,intrinsic same as in \p depth_to_flexion
/// \param[in] neighbors Number of neighboring pixels 
/// \param[out] flexion_image output image
/// \param[inout] flow taskgraph the calculations will be registered in
/// \returns synchronization task before and after the calculation
/// \note the calculation does not happen instantly but first a taskgraph is
/// \pre \p flexion_image has the same dimension as \p depth_image
/// \pre the underlying types match the defined template parameters
/// built. This graph will then execute all the tasks on request.
template <template <typename> typename Intrinsic, typename Real = float>
std::pair<tf::Task, tf::Task>
par_depth_to_flexion_nxn(const math::image<Real>& depth_image,
                         const Intrinsic<Real>&   intrinsic,
                         const int&               neighbors,
                         math::image<Real>&       flexion_image,
                         tf::Taskflow&            flow) noexcept;


namespace detail {
using ::sens_loc::math::vec;
template <template <typename> typename Intrinsic, typename Real = float>
inline void flexion_nxn_inner(int                      v,
                              const math::image<Real>& depth_image,
                              const Intrinsic<Real>&   intrinsic,
                              math::image<Real>&       out,
                              const int                n) {
    for (int u = n; u < depth_image.w() - n; ++u) {
        const Real d__1__0 = depth_image.at({u, v - n});
        const Real d_1__0  = depth_image.at({u, v + n});

        const Real d__0__1 = depth_image.at({u - n, v});
        const Real d__0_1  = depth_image.at({u + n, v});

        const Real d__1_1 = depth_image.at({u + n, v - n});
        const Real d_1__1 = depth_image.at({u - n, v + n});

        const Real d__1__1 = depth_image.at({u - n, v - n});
        const Real d_1_1   = depth_image.at({u + n, v + n});

        // If any of the depths is zero, the resulting vector will be the
        // null vector. This with then propagate through as zero and does not
        // induce any undefined behaviour or other problems.
        // Not short-circuiting results in easier vectorization / GPU
        // acceleration.

        using math::camera_coord;
        using math::pixel_coord;

        const camera_coord<Real> surface_pt0 =
            to_camera(intrinsic, {u, v - n}, d__1__0);
        const camera_coord<Real> surface_pt1 =
            to_camera(intrinsic, {u, v + n}, d_1__0);
        const camera_coord<Real> surface_dir0 = surface_pt1 - surface_pt0;

        const camera_coord<Real> surface_pt2 =
            to_camera(intrinsic, {u - n, v}, d__0__1);
        const camera_coord<Real> surface_pt3 =
            to_camera(intrinsic, {u + n, v}, d__0_1);
        const camera_coord<Real> surface_dir1 = surface_pt3 - surface_pt2;

        const camera_coord<Real> surface_pt4 =
            to_camera(intrinsic, {u + n, v - n}, d__1_1);
        const camera_coord<Real> surface_pt5 =
            to_camera(intrinsic, {u - n, v + n}, d_1__1);
        const camera_coord<Real> surface_dir2 = surface_pt5 - surface_pt4;

        const camera_coord<Real> surface_pt6 =
            to_camera(intrinsic, {u - n, v - n}, d__1__1);
        const camera_coord<Real> surface_pt7 =
            to_camera(intrinsic, {u + n, v + n}, d_1_1);
        const camera_coord<Real> surface_dir3 = surface_pt7 - surface_pt6;

        const auto cross0 =
            surface_dir0.normalized().cross(surface_dir1.normalized());
        const auto cross1 =
            surface_dir2.normalized().cross(surface_dir3.normalized());

        const auto flexion =
            std::clamp(std::abs(cross0.dot(cross1)), Real(0.), Real(1.));

        Ensures(flexion >= 0.);
        Ensures(flexion <= 1.);

        out.at({u, v}) = flexion;
    }
}

}  // namespace detail

template <template <typename> typename Intrinsic, typename Real>
inline math::image<Real>
depth_to_flexion_nxn(const math::image<Real>& depth_image,
                     const Intrinsic<Real>&   intrinsic,
                     const int&               neighbors) noexcept {
    static_assert(camera_models::is_intrinsic_v<Intrinsic, Real>);
    static_assert(std::is_floating_point_v<Real>);

    Expects(depth_image.w() == intrinsic.w());
    Expects(depth_image.h() == intrinsic.h());

    cv::Mat flexion(depth_image.h(), depth_image.w(),
                    math::detail::get_opencv_type<Real>());
    flexion = Real(0.);
    math::image<Real> flexion_image(std::move(flexion));
    for (int v = neighbors; v < depth_image.h() - neighbors; ++v)
        detail::flexion_nxn_inner(v, depth_image, intrinsic, flexion_image, neighbors);

    Ensures(flexion_image.w() == depth_image.w());
    Ensures(flexion_image.h() == depth_image.h());

    return flexion_image;
}

/// Convert an euclidian depth image to a flexion-image.
template <template <typename> typename Intrinsic, typename Real>
inline std::pair<tf::Task, tf::Task>
par_depth_to_flexion_nxn(const math::image<Real>& depth_image,
                         const Intrinsic<Real>&   intrinsic,
                         const int&               neighbors,
                         math::image<Real>&       flexion_image,
                         tf::Taskflow&            flow) noexcept {
    static_assert(camera_models::is_intrinsic_v<Intrinsic, Real>);
    static_assert(std::is_floating_point_v<Real>);

    Expects(depth_image.w() == intrinsic.w());
    Expects(depth_image.h() == intrinsic.h());

    Expects(flexion_image.w() == depth_image.w());
    Expects(flexion_image.h() == depth_image.h());

    auto sync_points = flow.parallel_for(
        neighbors, depth_image.h() - neighbors, 1, [&](int v) noexcept {
            detail::flexion_nxn_inner(v, depth_image, intrinsic, flexion_image, neighbors);
        });

    return sync_points;
}

}  // namespace sens_loc::conversion

#endif /* end of include guard: DEPTH_TO_TRIPLE_H_Y022ENVZ */
