template <typename Intrinsic>
bool gauss_curv_converter<Intrinsic>::process_file(
    const math::image<float>& depth_image, int idx) const noexcept {
    Expects(!this->_files.output.empty());
    using namespace conversion;

    const auto gauss =
        depth_to_gaussian_curvature(depth_image, this->intrinsic);
    bool success;
    if (this->_files.saveAs16Bit) {
        const auto converted = conversion::curvature_to_image<ushort>(
            gauss, depth_image, {lower_bound}, {upper_bound});
        success =
            cv::imwrite(fmt::format(this->_files.output, idx), converted.data());
    } else {
        const auto converted = conversion::curvature_to_image<uchar>(
            gauss, depth_image, {lower_bound}, {upper_bound});
        success =
            cv::imwrite(fmt::format(this->_files.output, idx), converted.data());
    }

    return success;
}

template <typename Intrinsic>
bool mean_curv_converter<Intrinsic>::process_file(
    const math::image<float>& depth_image, int idx) const noexcept {
    Expects(!this->_files.output.empty());
    using namespace conversion;

    const auto mean = depth_to_mean_curvature(depth_image, this->intrinsic);
    const auto converted = conversion::curvature_to_image<ushort>(
        mean, depth_image, {lower_bound}, {upper_bound});
    const bool success =
        cv::imwrite(fmt::format(this->_files.output, idx), converted.data());

    return success;
}
