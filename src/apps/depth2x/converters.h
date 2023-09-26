#ifndef CONVERTERS_H_HVFGCFVK
#define CONVERTERS_H_HVFGCFVK

#include <fmt/core.h>
#include <gsl/gsl>
#include <opencv2/imgcodecs.hpp>
#include <sens_loc/conversion/depth_to_bearing.h>
#include <sens_loc/conversion/depth_to_curvature.h>
#include <sens_loc/conversion/depth_to_flexion.h>
#include <sens_loc/conversion/depth_to_flexion_nxn.h>
#include <sens_loc/conversion/depth_to_flexion_normalized.h>
#include <sens_loc/conversion/depth_to_flexion_angle.h>
#include <sens_loc/conversion/depth_to_laserscan.h>
#include <sens_loc/conversion/depth_to_max_curve.h>
#include <util/batch_converter.h>

namespace sens_loc::apps {

/// \addtogroup conversion-driver
/// @{

/// Batch conversion to bearing-angle images.
/// \sa conversion::depth_to_bearing
template <typename Intrinsic>
class bearing_converter : public batch_sensor_converter<Intrinsic> {
  public:
    bearing_converter(const file_patterns& files,
                      depth_type           t,
                      Intrinsic            intrinsic)
        : batch_sensor_converter<Intrinsic>(files, t, std::move(intrinsic)) {
        if (files.horizontal.empty() && files.vertical.empty() &&
            files.diagonal.empty() && files.antidiagonal.empty()) {
            throw std::invalid_argument{
                "Missing output pattern for at least one bearing direction"};
        }
    }
    bearing_converter(const bearing_converter&) = default;
    bearing_converter(bearing_converter&&)      = default;
    bearing_converter& operator=(const bearing_converter&) = default;
    bearing_converter& operator=(bearing_converter&&) = default;
    ~bearing_converter() override                     = default;

  private:
    [[nodiscard]] bool process_file(const math::image<float>& depth_image,
                                    int idx) const noexcept override;
};
#include "converter_bearing.h.inl"

/// Convert orthographic depth-images to range (laserscan-like) images.
/// \sa conversion::depth_to_laserscan
template <typename Intrinsic>
class range_converter : public batch_sensor_converter<Intrinsic> {
  public:
    range_converter(const file_patterns& files,
                    depth_type           t,
                    Intrinsic            intrinsic)
        : batch_sensor_converter<Intrinsic>(files, t, std::move(intrinsic)) {}
    range_converter(const range_converter&) = default;
    range_converter(range_converter&&)      = default;
    range_converter& operator=(const range_converter&) = default;
    range_converter& operator=(range_converter&&) = default;
    ~range_converter() override                   = default;

  private:
    [[nodiscard]] bool process_file(const math::image<float>& depth_image,
                                    int idx) const noexcept override;
};
#include "converter_laserscan.h.inl"

/// Convert range-images to gaussian curvature images.
/// \sa conversion::depth_to_gaussian_curvature
template <typename Intrinsic>
class gauss_curv_converter : public batch_sensor_converter<Intrinsic> {
  public:
    /// \param files,t,intrinsic normal parameters for batch conversion
    /// \param lower_bound,upper_bound clamping parameters. Values below/above
    /// will map to these values.
    gauss_curv_converter(const file_patterns& files,
                         depth_type           t,
                         Intrinsic            intrinsic,
                         double               lower_bound,
                         double               upper_bound)
        : batch_sensor_converter<Intrinsic>(files, t, std::move(intrinsic))
        , lower_bound{lower_bound}
        , upper_bound{upper_bound} {}
    gauss_curv_converter(const gauss_curv_converter&) = default;
    gauss_curv_converter(gauss_curv_converter&&)      = default;
    gauss_curv_converter& operator=(const gauss_curv_converter&) = default;
    gauss_curv_converter& operator=(gauss_curv_converter&&) = default;
    ~gauss_curv_converter() override                        = default;

  private:
    [[nodiscard]] bool process_file(const math::image<float>& depth_image,
                                    int idx) const noexcept override;

    double lower_bound;
    double upper_bound;
};

/// Convert range-images to mean curvature images.
/// \sa conversion::depth_to_mean_curvature
template <typename Intrinsic>
class mean_curv_converter : public batch_sensor_converter<Intrinsic> {
  public:
    /// \param files,t,intrinsic normal parameters for batch conversion
    /// \param lower_bound,upper_bound clamping parameters. Values
    /// below/above will map to these values.
    mean_curv_converter(const file_patterns& files,
                        depth_type           t,
                        Intrinsic            intrinsic,
                        double               lower_bound,
                        double               upper_bound)
        : batch_sensor_converter<Intrinsic>(files, t, std::move(intrinsic))
        , lower_bound{lower_bound}
        , upper_bound{upper_bound} {}
    mean_curv_converter(const mean_curv_converter&) = default;
    mean_curv_converter(mean_curv_converter&&)      = default;
    mean_curv_converter& operator=(const mean_curv_converter&) = default;
    mean_curv_converter& operator=(mean_curv_converter&&) = default;
    ~mean_curv_converter() override                       = default;

  private:
    [[nodiscard]] bool process_file(const math::image<float>& depth_image,
                                    int idx) const noexcept override;

    double lower_bound;
    double upper_bound;
};
#include "converter_curvature.h.inl"

/// Convert range-images to max-curve images.
/// \sa conversion::depth_to_max_curve
template <typename Intrinsic>
class max_curve_converter : public batch_sensor_converter<Intrinsic> {
  public:
    max_curve_converter(const file_patterns& files,
                        depth_type           t,
                        Intrinsic            intrinsic)
        : batch_sensor_converter<Intrinsic>(files, t, std::move(intrinsic)) {}
    max_curve_converter(const max_curve_converter&) = default;
    max_curve_converter(max_curve_converter&&)      = default;
    max_curve_converter& operator=(const max_curve_converter&) = default;
    max_curve_converter& operator=(max_curve_converter&&) = default;
    ~max_curve_converter() override                       = default;

  private:
    [[nodiscard]] bool process_file(const math::image<float>& depth_image,
                                    int idx) const noexcept override;
};
#include "converter_max_curve.h.inl"

/// Convert range-images to flexion images.
/// \sa conversion::depth_to_flexion
template <typename Intrinsic>
class flexion_converter : public batch_sensor_converter<Intrinsic> {
  public:
    flexion_converter(const file_patterns& files,
                      depth_type           t,
                      Intrinsic            intrinsic)
        : batch_sensor_converter<Intrinsic>(files, t, std::move(intrinsic)) {}
    flexion_converter(const flexion_converter&) = default;
    flexion_converter(flexion_converter&&)      = default;
    flexion_converter& operator=(const flexion_converter&) = default;
    flexion_converter& operator=(flexion_converter&&) = default;
    ~flexion_converter() override                     = default;

  private:
    [[nodiscard]] bool process_file(const math::image<float>& depth_image,
                                    int idx) const noexcept override;
};
#include "converter_flexion.h.inl"

/// Convert range-images to flexion images.
/// \sa conversion::depth_to_flexion
template <typename Intrinsic>
class flexion_converter_nxn : public batch_sensor_converter<Intrinsic> {
  public:
    flexion_converter_nxn(const file_patterns& files,
                          depth_type           t,
                          Intrinsic            intrinsic)
        : batch_sensor_converter<Intrinsic>(files, t, std::move(intrinsic)) {}
    flexion_converter_nxn(const flexion_converter_nxn&) = default;
    flexion_converter_nxn(flexion_converter_nxn&&)      = default;
    flexion_converter_nxn& operator=(const flexion_converter_nxn&) = default;
    flexion_converter_nxn& operator=(flexion_converter_nxn&&) = default;
    ~flexion_converter_nxn() override                     = default;

  private:
    [[nodiscard]] bool process_file(const math::image<float>& depth_image,
                                    int idx) const noexcept override;
};
#include "converter_flexion_nxn.h.inl"

/// Convert range-images to flexion images.
/// \sa conversion::depth_to_flexion
template <typename Intrinsic>
class flexion_converter_normalized : public batch_sensor_converter<Intrinsic> {
  public:
    flexion_converter_normalized(const file_patterns& files,
                                 depth_type           t,
                                 Intrinsic            intrinsic)
        : batch_sensor_converter<Intrinsic>(files, t, std::move(intrinsic)) {}
    flexion_converter_normalized(const flexion_converter_normalized&) = default;
    flexion_converter_normalized(flexion_converter_normalized&&)      = default;
    flexion_converter_normalized& operator=(const flexion_converter_normalized&) = default;
    flexion_converter_normalized& operator=(flexion_converter_normalized&&) = default;
    ~flexion_converter_normalized() override                     = default;

  private:
    [[nodiscard]] bool process_file(const math::image<float>& depth_image,
                                    int idx) const noexcept override;
};
#include "converter_flexion_normalized.h.inl"

/// Convert range-images to flexion images.
/// \sa conversion::depth_to_flexion
template <typename Intrinsic>
class flexion_converter_angle : public batch_sensor_converter<Intrinsic> {
  public:
    flexion_converter_angle(const file_patterns& files,
                            depth_type           t,
                            Intrinsic            intrinsic)
        : batch_sensor_converter<Intrinsic>(files, t, std::move(intrinsic)) {}
    flexion_converter_angle(const flexion_converter_angle&) = default;
    flexion_converter_angle(flexion_converter_angle&&)      = default;
    flexion_converter_angle& operator=(const flexion_converter_angle&) = default;
    flexion_converter_angle& operator=(flexion_converter_angle&&) = default;
    ~flexion_converter_angle() override                     = default;

  private:
    [[nodiscard]] bool process_file(const math::image<float>& depth_image,
                                    int idx) const noexcept override;
};
#include "converter_flexion_angle.h.inl"

/// @}

}  // namespace sens_loc::apps

#endif
