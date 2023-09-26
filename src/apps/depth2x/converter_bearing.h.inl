template <typename Intrinsic>
bool bearing_converter<Intrinsic>::process_file(
    const math::image<float>& depth_image, int idx) const noexcept {
    Expects(!this->_files.horizontal.empty() ||
            !this->_files.vertical.empty() || !this->_files.diagonal.empty() ||
            !this->_files.antidiagonal.empty());
    using namespace sens_loc::conversion;

    bool final_result = true;

#define BEARING_PROCESS(DIRECTION)                                             \
    if (!this->_files.DIRECTION.empty()) {                                     \
        math::image<float> bearing = depth_to_bearing<direction::DIRECTION>(   \
            depth_image, this->intrinsic);                                     \
        cv::Mat img;                                                           \
        if (this->_files.saveAs16Bit) {                                        \
            img = convert_bearing<float, ushort>(bearing).data();              \
        } else {                                                               \
            img = convert_bearing<float, uchar>(bearing).data();               \
        }                                                                      \
        bool success =                                                         \
            cv::imwrite(fmt::format(this->_files.DIRECTION, idx), img);        \
        final_result &= success;                                               \
    }

    BEARING_PROCESS(horizontal)
    BEARING_PROCESS(vertical)
    BEARING_PROCESS(diagonal)
    BEARING_PROCESS(antidiagonal)

#undef BEARING_PROCESS

    return final_result;
}
