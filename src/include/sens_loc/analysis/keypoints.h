#ifndef KEYPOINTS_H_CFNIGHAG
#define KEYPOINTS_H_CFNIGHAG

#include <boost/histogram.hpp>
#include <gsl/gsl>
#include <opencv2/core/persistence.hpp>
#include <opencv2/core/types.hpp>
#include <sens_loc/analysis/distance.h>

namespace sens_loc::analysis {

/// Analyze the distribution and characteristics of detected keypoints.
class keypoints {
  public:
    /// Use a regular grid with no transformation, no overflow and an axis
    /// title.
    using axis_t = distance::axis_t;
    /// Histogram type for the 2D distribution of keypoints in the dataset.
    using distribution_histo_t =
        decltype(boost::histogram::make_histogram(axis_t{}, axis_t{}));
    /// Histogram type for 'size' and 'response' of the keypoints.
    using histo_t = decltype(boost::histogram::make_histogram(axis_t{}));

    keypoints() = default;
    keypoints(unsigned int img_width, unsigned int img_height)
        : _img_width{img_width}
        , _img_height{img_height} {}

    void configure_image_dimension(unsigned int img_width,
                                   unsigned int img_height) noexcept {
        _img_width  = img_width;
        _img_height = img_height;
    }

    void configure_size(unsigned int bins) noexcept { _size_bins = bins; }
    void configure_size(std::string axis_title) noexcept {
        _size_title = std::move(axis_title);
    }
    void configure_size(unsigned int bins, std::string axis_title) noexcept {
        _size_bins  = bins;
        _size_title = std::move(axis_title);
    }
    void enable_size_histo(bool enabled) noexcept {
        _size_histo_enabled = enabled;
    }


    void configure_response(unsigned int bins) noexcept {
        _response_bins = bins;
    }
    void configure_response(std::string axis_title) noexcept {
        _response_title = std::move(axis_title);
    }
    void configure_response(unsigned int bins,
                            std::string  axis_title) noexcept {
        _response_bins  = bins;
        _response_title = std::move(axis_title);
    }
    void enable_response_histo(bool enabled) noexcept {
        _response_histo_enabled = enabled;
    }


    void configure_distribution(unsigned int both_bins) noexcept {
        _dist_width_bins  = both_bins;
        _dist_height_bins = both_bins;
    }
    void configure_distribution(unsigned int w_bins,
                                unsigned int h_bins) noexcept {
        _dist_width_bins  = w_bins;
        _dist_height_bins = h_bins;
    }
    void configure_distribution(std::string w_axis_title,
                                std::string h_axis_title) noexcept {
        _dist_w_title = std::move(w_axis_title);
        _dist_h_title = std::move(h_axis_title);
    }

    /// Analyze the properties of a set of keypoints. The booleans are toggles
    /// to deactivate analysis for the specified quantity to save some
    /// computations.
    void analyze(gsl::span<const cv::KeyPoint> points,
                 bool                          distribution = true,
                 bool                          size         = true,
                 bool                          response     = true) noexcept;

    [[nodiscard]] const distribution_histo_t& distribution() const noexcept {
        return _distribution;
    }
    [[nodiscard]] const histo_t& size_histo() const noexcept {
        return _size_histo;
    }
    [[nodiscard]] const histo_t& response_histo() const noexcept {
        return _response_histo;
    }

    [[nodiscard]] const statistic& size() const noexcept { return _size; }
    [[nodiscard]] const statistic& response() const noexcept {
        return _response;
    }

  private:
    unsigned int _img_width  = 0U;
    unsigned int _img_height = 0U;

    bool         _size_histo_enabled = true;
    statistic    _size;
    histo_t      _size_histo;
    unsigned int _size_bins  = 50U;
    std::string  _size_title = "size of keypoints";

    bool         _response_histo_enabled = true;
    statistic    _response;
    histo_t      _response_histo;
    unsigned int _response_bins  = 50U;
    std::string  _response_title = "response of keypoints";

    distribution_histo_t _distribution;
    unsigned int         _dist_width_bins  = 50U;
    unsigned int         _dist_height_bins = 50U;
    std::string          _dist_w_title = "width - distribution of keypoints";
    std::string          _dist_h_title = "height - distribution of keypoints";
};

/// Write the statistics for the keypoint distribution in a file with OpenCVs
/// FileStorage API.
void write(cv::FileStorage& fs, const std::string& name, const keypoints& kp);

}  // namespace sens_loc::analysis

#endif /* end of include guard: KEYPOINTS_H_CFNIGHAG */
