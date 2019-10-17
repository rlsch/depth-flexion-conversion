#ifndef DEPTH_TO_LASERSCAN_H_P8V9HAVF
#define DEPTH_TO_LASERSCAN_H_P8V9HAVF

#include <opencv2/core/mat.hpp>
#include <sens_loc/camera_models/pinhole.h>
#include <sens_loc/conversion/util.h>
#include <taskflow/taskflow.hpp>

namespace sens_loc { namespace conversion {

/// This function converts an orthographic depth image to an laser-scan like
/// depth image, aka. range-image.
///
/// Each pixel in the resulting laser-scan image describes the euclidean
/// distance from the camera center to the point in space.
/// The result type is 'Real'.
/// \tparam Real precision of the calculation
/// \tparam PixelType underlying type of the input image
/// \param depth_image orthgraphic depth image, e.g. from a kinect
/// \param intrinsic matching calibration of the sensor
/// \returns matrix with each value converted to the euclidean distance
/// \note invalid values (where the depth is zero) will be zero as well
/// \post each value is bigger or equal to the original pixel value
template <typename Real = float, typename PixelType = ushort>
cv::Mat depth_to_laserscan(const cv::Mat &               depth_image,
                           const camera_models::pinhole &intrinsic) noexcept;

/// This function is the parallel implementation for the conversions.
/// \sa conversion::depth_to_laserscan
/// \param[in] depth_image,intrinsic same as in serial case
/// \param[out] out resulting converted image
/// \param[inout] flow taskgraph that will be used for the parallel jobs
/// \returns synchronization tasks before and after the conversion.
template <typename Real = float, typename PixelType = ushort>
std::pair<tf::Task, tf::Task>
par_depth_to_laserscan(const cv::Mat &               depth_image,
                       const camera_models::pinhole &intrinsic, cv::Mat &out,
                       tf::Taskflow &flow) noexcept;

namespace detail {
template <typename Real, typename PixelType>
void laserscan_inner(const int v, const cv::Mat &depth_image,
                     const camera_models::pinhole &intrinsic, cv::Mat &euclid) {
    for (int u = 0; u < depth_image.cols; ++u) {
        const PixelType d_o = depth_image.at<PixelType>(v, u);
        euclid.at<Real>(v, u) =
            orthografic_to_euclidian<Real>(u, v, d_o, intrinsic);
    }
}
}  // namespace detail

template <typename Real, typename PixelType>
inline cv::Mat
depth_to_laserscan(const cv::Mat &               depth_image,
                   const camera_models::pinhole &intrinsic) noexcept {
    Expects(depth_image.channels() == 1);
    Expects(!depth_image.empty());
    Expects(depth_image.cols > 2);
    Expects(depth_image.rows > 2);

    using namespace detail;
    cv::Mat euclid(depth_image.rows, depth_image.cols, get_cv_type<Real>());

    for (int v = 0; v < depth_image.rows; ++v) {
        detail::laserscan_inner<Real, PixelType>(v, depth_image, intrinsic,
                                                 euclid);
    }

    Ensures(euclid.rows == depth_image.rows);
    Ensures(euclid.cols == depth_image.cols);
    Expects(euclid.channels() == 1);
    Expects(euclid.type() == get_cv_type<Real>());

    return euclid;
}


template <typename Real, typename PixelType>
inline std::pair<tf::Task, tf::Task>
par_depth_to_laserscan(const cv::Mat &               depth_image,
                       const camera_models::pinhole &intrinsic, cv::Mat &out,
                       tf::Taskflow &flow) noexcept {
    Expects(depth_image.channels() == 1);
    Expects(!depth_image.empty());
    Expects(depth_image.cols > 2);
    Expects(depth_image.rows > 2);
    Expects(out.rows == depth_image.rows);
    Expects(out.cols == depth_image.cols);
    Expects(out.channels() == depth_image.channels());
    Expects(out.type() == detail::get_cv_type<Real>());

    auto sync_points = flow.parallel_for(0, depth_image.rows, 1, [&](int v) {
        detail::laserscan_inner<Real, PixelType>(v, depth_image, intrinsic,
                                                 out);
    });

    return sync_points;
}
}}  // namespace sens_loc::conversion

#endif /* end of include guard: DEPTH_TO_LASERSCAN_H_P8V9HAVF */
