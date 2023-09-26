template <typename Intrinsic>
bool max_curve_converter<Intrinsic>::process_file(
    const math::image<float>& depth_image, int idx) const noexcept {
    Expects(!this->_files.output.empty());
    using namespace conversion;

    const auto max_curve = depth_to_max_curve(depth_image, this->intrinsic);
    cv::Mat img;
    if (this->_files.saveAs16Bit) {
        img = convert_max_curve<ushort>(max_curve).data();
    } else {
        img = convert_max_curve<uchar>(max_curve).data();
    }
    const bool success =
        cv::imwrite(fmt::format(this->_files.output, idx), img);

    return success;
}
