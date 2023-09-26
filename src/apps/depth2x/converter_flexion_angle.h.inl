template <typename Intrinsic>
bool flexion_converter_angle<Intrinsic>::process_file(
    const math::image<float>& depth_image, int idx) const noexcept {
    Expects(!this->_files.output.empty());
    using namespace conversion;

    const auto flexion = depth_to_flexion_angle(depth_image, this->intrinsic);
    cv::Mat img;
    if (this->_files.saveAs16Bit) {
        img = convert_flexion<ushort>(flexion).data();
    } else {
        img = convert_flexion<uchar>(flexion).data();
    }
    const bool success = cv::imwrite(fmt::format(this->_files.output, idx), img);

    return success;
}
