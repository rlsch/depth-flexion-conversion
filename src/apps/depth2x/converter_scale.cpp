#include "converter_scale.h"

#include <fmt/core.h>
#include <opencv2/imgcodecs.hpp>
#include <sens_loc/conversion/depth_scaling.h>

namespace sens_loc::apps {

bool scale_converter::process_file(const math::image<float>& depth_image,
                                   int idx) const noexcept {
    Expects(!_files.output.empty());
    using namespace sens_loc::conversion;
    const auto res = depth_scaling(depth_image, _scale, _offset);
    cv::Mat depth;
    if (this->_files.saveAs16Bit) {
        depth = cv::Mat(depth_image.h(), depth_image.w(), CV_16U);
        res.data().convertTo(depth, CV_16U);
    } else {
        depth = cv::Mat(depth_image.h(), depth_image.w(), CV_8U);
        res.data().convertTo(depth, CV_8U);
    }

    bool success = cv::imwrite(fmt::format(_files.output, idx), depth);

    return success;
}

}  // namespace sens_loc::apps
