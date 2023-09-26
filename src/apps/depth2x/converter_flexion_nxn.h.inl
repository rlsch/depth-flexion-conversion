template <typename Intrinsic>
bool flexion_converter_nxn<Intrinsic>::process_file(
    const math::image<float>& depth_image, int idx) const noexcept {
    Expects(!this->_files.output.empty());
    Expects(!this->_files.neighbors.empty());
    using namespace conversion;

    const auto flexion = depth_to_flexion_nxn(depth_image, this->intrinsic, std::stoi(this->_files.neighbors));
    cv::Mat img;
    if (this->_files.saveAs16Bit) {
        img = convert_flexion<ushort>(flexion).data();
    } else {
        img = convert_flexion<uchar>(flexion).data();
    }
    const bool success = cv::imwrite(fmt::format(this->_files.output, idx), img);

    return success;
}
