template <typename Intrinsic>
bool range_converter<Intrinsic>::process_file(
    const math::image<float>& depth_image, int idx) const noexcept {
    Expects(!this->_files.output.empty());
    using namespace conversion;

    /// The input 'depth_image' is already in range-form as its beeing
    /// preprocessed.
    cv::Mat depth;
    if (this->_files.saveAs16Bit) {
        depth = cv::Mat(depth_image.h(), depth_image.w(), CV_16U);
        depth_image.data().convertTo(depth, CV_16U);
    } else {
        depth = cv::Mat(depth_image.h(), depth_image.w(), CV_8U);
        depth_image.data().convertTo(depth, CV_8U);
    }
    bool success =
        cv::imwrite(fmt::format(this->_files.output, idx), depth);

    return success;
}
