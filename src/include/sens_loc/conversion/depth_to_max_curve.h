#ifndef DEPTH_TO_MAX_CURVE_H_XO6PUN8H
#define DEPTH_TO_MAX_CURVE_H_XO6PUN8H

#include <cmath>
#include <opencv2/core/mat.hpp>
#include <sens_loc/camera_models/pinhole.h>
#include <sens_loc/conversion/depth_to_bearing.h>
#include <sens_loc/conversion/util.h>
#include <sens_loc/math/constants.h>
#include <sens_loc/math/triangles.h>

namespace sens_loc { namespace conversion {

/// Convert a range image to an image derived from bearing angles.
///
/// Instead of calculating the bearing angle only in one direction, both
/// neighbours are considered for a certain direction and the full angle is
/// calculated. This is done for all directions (vertical, horziontal,
/// diagonals) and the maximum angle is then used as value.
///
/// \sa conversion::depth_to_laserscan
/// \sa conversion::depth_to_bearing
/// \tparam Real precision of the calculation
/// \tparam PixelType underlying type of \p depth_image
/// \param depth_image range image the calculations are made with
/// \param intrinsic matching calibration for the sensor
/// \returns the max-curve image with each pixel in the range \f$[0, 2\pi)\f$
/// \pre \p depth_image is not empty
/// \pre the calibration matches the sensor that took the picture
/// \pre the underlying types match
/// \post each pixel has a value in the range \f$[0, 2\pi)\f$
/// \post the values are provided in radians
template <typename Real = float, typename PixelType = float>
cv::Mat depth_to_max_curve(const cv::Mat &               depth_image,
                           const camera_models::pinhole &intrinsic) noexcept;

/// The max-curve picture is not a normal image and needs to be converted to
/// the classical integer range.
///
/// \tparam Real underlying type of \p max_curve
/// \tparam PixelType underlying type of the result
/// \param max_curve max-curve image
/// \returns scaled image with underlying type \p PixelType
/// \pre each pixel of \p max_curve is in range \f$[0, 2\pi)\f$
/// \post each pixel is in range \f$[PixelType_{min}, PixelType_{max}]\f$
template <typename Real = float, typename PixelType = ushort>
cv::Mat convert_max_curve(const cv::Mat &max_curve) noexcept;

namespace detail {

template <typename Real>  // require Float<Real>
inline Real angle_formula(const Real d__1, const Real d__0, const Real d_1,
                          const Real cos_alpha1,
                          const Real cos_alpha2) noexcept {
    if (d__1 == 0. || d__0 == 0. || d_1 == 0.)
        return Real(0.);

    // => alpha is smaller 90°
    // => alpha is bigger 0°
    Expects(cos_alpha1 > 0.);
    Expects(cos_alpha1 < 1.);
    Expects(cos_alpha2 > 0.);
    Expects(cos_alpha2 < 1.);

    Expects(d__1 > 0.);
    Expects(d__0 > 0.);
    Expects(d_1 > 0.);

    const Real angle = math::bearing_angle(d__0, d__1, cos_alpha1) +
                       math::bearing_angle(d__0, d_1, cos_alpha2);

    Ensures(angle > 0.);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    Ensures(angle < 2. * math::pi<Real>);

    return angle;
}
}  // namespace detail

template <typename Real, typename PixelType>
inline cv::Mat
depth_to_max_curve(const cv::Mat &               depth_image,
                   const camera_models::pinhole &intrinsic) noexcept {
    using namespace detail;

    Expects(depth_image.type() == get_cv_type<PixelType>());
    Expects(depth_image.channels() == 1);
    Expects(!depth_image.empty());
    Expects(depth_image.cols > 2);
    Expects(depth_image.rows > 2);

    cv::Mat max_curve(depth_image.rows, depth_image.cols, get_cv_type<Real>());

    for (int v = 1; v < depth_image.rows - 1; ++v) {
        for (int u = 1; u < depth_image.cols - 1; ++u) {
            const Real d__1__1 = depth_image.at<PixelType>(v - 1, u - 1);
            const Real d__1__0 = depth_image.at<PixelType>(v - 1, u);
            const Real d__1_1  = depth_image.at<PixelType>(v - 1, u + 1);

            const Real d__0__1 = depth_image.at<PixelType>(v, u - 1);
            const Real d__0__0 = depth_image.at<PixelType>(v, u);
            const Real d__0_1  = depth_image.at<PixelType>(v, u + 1);

            const Real d_1__1 = depth_image.at<PixelType>(v + 1, u - 1);
            const Real d_1__0 = depth_image.at<PixelType>(v + 1, u);
            const Real d_1_1  = depth_image.at<PixelType>(v + 1, u + 1);

            using std::cos;
            const Real phi_hor1  = intrinsic.phi(u - 1, v, u, v);
            const Real phi_hor2  = intrinsic.phi(u, v, u + 1, v);
            const Real angle_hor = angle_formula(d__0__1, d__0__0, d__0_1,
                                                 cos(phi_hor1), cos(phi_hor2));

            // vertical angular resolution
            const Real phi_ver1  = intrinsic.phi(u, v - 1, u, v);
            const Real phi_ver2  = intrinsic.phi(u, v, u, v + 1);
            const Real angle_ver = angle_formula(d__1__0, d__0__0, d_1__0,
                                                 cos(phi_ver1), cos(phi_ver2));

            // diagonal angular resolution
            const Real phi_dia1  = intrinsic.phi(u - 1, v - 1, u, v);
            const Real phi_dia2  = intrinsic.phi(u, v, u + 1, v + 1);
            const Real angle_dia = angle_formula(d__1__1, d__0__0, d_1_1,
                                                 cos(phi_dia1), cos(phi_dia2));

            // antidiagonal angular resolution
            const Real phi_ant1  = intrinsic.phi(u + 1, v + 1, u, v);
            const Real phi_ant2  = intrinsic.phi(u, v, u + 1, v - 1);
            const Real angle_ant = angle_formula(d_1__1, d__0__0, d__1_1,
                                                 cos(phi_ant1), cos(phi_ant2));

            using std::max;
            const Real max_angle =
                max(angle_hor, max(angle_ver, max(angle_dia, angle_ant)));

            Ensures(max_angle >= 0.);
            // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
            Ensures(max_angle < 2. * math::pi<Real>);
            max_curve.at<Real>(v, u) = max_angle;
        }
    }
    Ensures(max_curve.cols == depth_image.cols);
    Ensures(max_curve.rows == depth_image.rows);
    Ensures(max_curve.type() == get_cv_type<Real>());
    Ensures(max_curve.channels() == 1);

    return max_curve;
}

template <typename Real, typename PixelType>
inline cv::Mat convert_max_curve(const cv::Mat &max_curve) noexcept {
    using namespace detail;

    Expects(max_curve.channels() == 1);
    Expects(max_curve.rows > 2);
    Expects(max_curve.cols > 2);
    Expects(max_curve.type() == get_cv_type<Real>());

    cv::Mat img(max_curve.rows, max_curve.cols, get_cv_type<PixelType>());
    auto [scale, offset] =
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        scaling_factor<Real, PixelType>(/*max_angle = */ 2. * math::pi<Real>);
    max_curve.convertTo(img, get_cv_type<PixelType>(), scale, offset);

    Ensures(img.cols == max_curve.cols);
    Ensures(img.rows == max_curve.rows);
    Ensures(img.type() == get_cv_type<PixelType>());
    Ensures(img.channels() == 1);

    return img;
}
}}  // namespace sens_loc::conversion

#endif /* end of include guard: DEPTH_TO_MAX_CURVE_H_XO6PUN8H */
