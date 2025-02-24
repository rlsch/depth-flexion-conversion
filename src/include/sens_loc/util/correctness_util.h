#ifndef CORRECTNESS_UTIL_H_BJPG9UMQ
#define CORRECTNESS_UTIL_H_BJPG9UMQ

#include <opencv2/core.hpp>
#include <sens_loc/math/image.h>
#include <string_view>

namespace sens_loc {

/// This namespace is purely for programming utility.
namespace util {


#define UNREACHABLE(msg) ::sens_loc::util::unreachable(msg, __FILE__, __LINE__)

/// This function is used with the \c UNREACHABLE macro to symbol unreachable
/// code paths.
///
/// If such a code-path is reached on execution and error message will be
/// printed, that includes the location of the \c unreachable and the program
/// terminates with \c std::exit(1)
///
/// \code
/// switch(my_value) {
///   case 1: // FOO
///   case 2: // BAR
/// }
/// UNREACHABLE("Switch was exhaustive, invalid argument passed");
/// \endcode
///
/// \note \c unreachable is inspired by \c llvm_unreachable
/// \note use  `// LCOV_EXCL_LINE` to exclude UNREACHABLE from the coverage
/// reports
[[noreturn]] void unreachable(std::string_view message = {},
                              std::string_view file    = {},
                              int              line    = -1) noexcept;

/// Calculate the average pixel error of the two images.
/// This function uses the L2-Norm as error measure and divides it by the
/// number of pixels.
///
/// \pre Expects that both images are of same underlying type and dimension.
/// \post The results is bigger or equal to 0.
///
/// \returns
/// \f[\frac{\lVert i_1 - i_2 \rVert_2}{\#Pixel \cdot \#Channels}\f]
double average_pixel_error(const cv::Mat& i1, const cv::Mat& i2) noexcept;

template <typename Image>
double average_pixel_error(const Image& i1, const Image& i2) noexcept {
    return average_pixel_error(i1.data(), i2.data());
}

}  // namespace util
}  // namespace sens_loc

#endif /* end of include guard: CORRECTNESS_UTIL_H_BJPG9UMQ */
