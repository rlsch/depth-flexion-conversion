#ifndef BATCH_CONVERTER_H_XDIRBPHG
#define BATCH_CONVERTER_H_XDIRBPHG

#include <optional>
#include <sens_loc/camera_models/concepts.h>
#include <sens_loc/conversion/depth_to_laserscan.h>
#include <sens_loc/math/image.h>
#include <sens_loc/util/correctness_util.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

namespace sens_loc {

/// This namespace includes all code that is only relevant to executables
/// but does not provide library functionality.
namespace apps {

/// \addtogroup conversion-driver
/// @{

/// Discriminate the input data semantic between depth-maps and laserscans.
enum class depth_type {
    orthografic,  ///< The \c orthographic distance measure the distance to the
                  ///< image plan. Original Kinect images use this as depth
                  ///< value.
    euclidean,    ///< The \c euclidean distance is the L2-Norm from the point
                  ///< in camera coordinates to the camera center.
};

/// Helper function for the conversion from command-line argument to typesafe
/// enumeration type.
/// \sa depth_type
/// \pre \p option is one of "pinhole-depth" or "pinhole-range"
/// \note The precondition of the function should be established by the command
/// line parser configuration.
/// \returns the matching enumerator for the given option.
inline depth_type str_to_depth_type(std::string_view option) {
    if (option == "pinhole-depth")
        return depth_type::orthografic;
    if (option == "pinhole-range")
        return depth_type::euclidean;

    UNREACHABLE(                                              // LCOV_EXCL_LINE
        "no other options are allowed as input depth type");  // LCOV_EXCL_LINE
}

/// Helper struct to contain path specification for input and output files.
/// The paths can be patterns like \c "depth-{}.png" where curly braces are
/// substituted with an index.
struct file_patterns {
    std::string input;         ///< Input file pattern specification.
    std::string output;        ///< Output file pattern specification.
    std::string horizontal;    ///< Only relevant for bearing angles, output for
                               ///< horizontal images.
    std::string vertical;      ///< Only relevant for bearing angles, output for
                               ///< vertical images.
    std::string diagonal;      ///< Only relevant for bearing angles, output for
                               ///< diagonal images.
    std::string antidiagonal;  ///< Only relevant for bearing angles, output for
                               ///< antidiagonal images.
    bool saveAs16Bit;          ///< Bit depth of output image, 1 = 16bit, 0 = 8bit
    std::string neighbors;     ///< Only relevant for flexion images, output for
                               ///< images with nxn neighborhood.
};

/// Just local helper for batch conversion tasks over a given index range.
/// Provides abstract interface for batch processing depth images for different
/// tasks.
class batch_converter {
  public:
    batch_converter(const file_patterns& files)
        : _files{files} {
        Expects(!_files.input.empty());
    }

    batch_converter(const batch_converter&)            = default;
    batch_converter(batch_converter&&)                 = default;
    batch_converter& operator=(const batch_converter&) = default;
    batch_converter& operator=(batch_converter&&)      = default;

    /// Process the whole batch calling 'process_file' for each index.
    /// \note This function does parallel batch processing.
    /// \note As a high level function it catches all exceptions and provides
    /// human readable error message to std-out.
    /// \returns 'false' if any of the indices fails.
    [[nodiscard]] bool process_batch(int start, int end) const noexcept;

    virtual ~batch_converter() = default;

  protected:
    file_patterns _files;  ///< File patterns that shall be processed.

  private:
    /// Function that does the management-tasks for the conversion job, like
    /// file-io and error handling.
    ///
    /// The function opens the \c _files.input file after index substitution
    /// and reads it as an 16-bit, single channel grayscale image.
    /// If it does not succeed, it returns \c false.
    ///
    /// On success it calls \p process_file which implements the actual
    /// conversion in each subclass.
    ///
    /// \sa process_file
    /// \pre \p _files.input is not empty
    /// \returns \c true on success, otherwise \c false.
    [[nodiscard]] bool process_index(int idx) const noexcept;

    /// Function to potentially convert orthographic images into range images.
    /// \returns \c cv::Mat with proper input data for the conversion process.
    [[nodiscard]] virtual std::optional<math::image<float>>
    preprocess_depth(const math::image<ushort>& depth_image) const noexcept;

    /// Method to process exactly one file. This method is expected to have
    /// no sideeffects and is called in parallel.
    /// \returns \c true on success, otherwise \c false.
    [[nodiscard]] virtual bool
    process_file(const math::image<float>& depth_image,
                 int                       idx) const noexcept = 0;
};

/// This class provides common data and depth-image conversion for all
/// pinhole-camera based input images.
template <typename Intrinsic>
class batch_sensor_converter : public batch_converter {
  public:
    batch_sensor_converter(const file_patterns& files,
                           depth_type           t,
                           Intrinsic            intrinsic)
        : batch_converter(files)
        , intrinsic{std::move(intrinsic)}
        , _input_depth_type{t} {}

    batch_sensor_converter(const batch_sensor_converter&)            = default;
    batch_sensor_converter(batch_sensor_converter&&)                 = default;
    batch_sensor_converter& operator=(const batch_sensor_converter&) = default;
    batch_sensor_converter& operator=(batch_sensor_converter&&)      = default;
    ~batch_sensor_converter() override                               = default;

  protected:
    /// pinhole-camera-model parameters used in the whole conversion.
    Intrinsic intrinsic;
    /// Discriminate input type of the images.
    depth_type _input_depth_type;

  private:
    /// Convert orthographic depth-images to range images using the pinhole
    /// model.
    /// \sa conversion::depth_to_laserscan
    /// \returns \c cv::Mat with one channel and double as data type.
    [[nodiscard]] std::optional<math::image<float>> preprocess_depth(
        const math::image<ushort>& depth_image) const noexcept override {
        if ((depth_image.w() != intrinsic.w()) ||
            depth_image.h() != intrinsic.h())
            return std::nullopt;

        switch (_input_depth_type) {
        case depth_type::orthografic:
            return conversion::depth_to_laserscan<float, ushort>(depth_image,
                                                                 intrinsic);
        case depth_type::euclidean: return math::convert<float>(depth_image);
        }
        UNREACHABLE("Switch is exhaustive");  // LCOV_EXCL_LINE
    }
};
/// @}

}  // namespace apps
}  // namespace sens_loc

#endif /* end of include guard: BATCH_CONVERTER_H_XDIRBPHG */
