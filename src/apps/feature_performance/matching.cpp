#include "matching.h"

#include <boost/histogram/ostream.hpp>
#include <cstdint>
#include <fstream>
#include <gsl/gsl>
#include <iterator>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <sens_loc/analysis/distance.h>
#include <sens_loc/io/histogram.h>
#include <sens_loc/io/image.h>
#include <sens_loc/util/console.h>
#include <util/batch_visitor.h>
#include <util/statistic_visitor.h>

using namespace cv;
using namespace std;
using namespace gsl;

namespace {
class matching {
  public:
    matching(not_null<mutex*>         distance_mutex,
             not_null<vector<float>*> distances,
             not_null<uint64_t*>      descriptor_count,
             NormTypes                norm_to_use,
             bool                     crosscheck,
             string_view              input_pattern,
             optional<string_view>    output_pattern,
             optional<string_view>    original_files) noexcept
        : distance_mutex{distance_mutex}
        , distances{distances}
        , total_descriptors{descriptor_count}
        , matcher{BFMatcher::create(norm_to_use, crosscheck)}
        , input_pattern{input_pattern}
        , output_pattern{output_pattern}
        , original_images{original_files} {
        // XOR is true if both operands have the same value.
        Expects(!(output_pattern.has_value() ^ original_images.has_value()) &&
                "Either both or none are set");
    }

    void operator()(int idx,
                    optional<vector<KeyPoint>> /*keypoints*/,  // NOLINT
                    optional<Mat> descriptors) noexcept {
        if (!descriptors || (descriptors->rows == 0))
            return;
        try {
            const int         previous_idx = idx - 1;
            const FileStorage previous_img = sens_loc::io::open_feature_file(
                fmt::format(input_pattern, previous_idx));
            Mat previous_descriptors =
                sens_loc::io::load_descriptors(previous_img);

            vector<DMatch> matches;
            matcher->match(*descriptors, previous_descriptors, matches);

            {
                lock_guard guard{*distance_mutex};
                // Insert all the distances into the global distances vector.
                transform(begin(matches), end(matches),
                          back_inserter(*distances),
                          [](const DMatch& m) { return m.distance; });
                (*total_descriptors) += descriptors->rows;
            }

            if (output_pattern) {
                const FileStorage this_feature =
                    sens_loc::io::open_feature_file(
                        fmt::format(input_pattern, idx));
                const vector<KeyPoint> previous_keypoints =
                    sens_loc::io::load_keypoints(this_feature);

                const vector<KeyPoint> this_keypoints =
                    sens_loc::io::load_keypoints(previous_img);

                const string img_p1 = fmt::format(*original_images, idx - 1);
                const string img_p2 = fmt::format(*original_images, idx);
                auto         img1   = sens_loc::io::load_as_8bit_gray(img_p1);
                auto         img2   = sens_loc::io::load_as_8bit_gray(img_p2);

                if (!img1 || !img2)
                    return;

                Mat out_img;
                drawMatches(img1->data(), previous_keypoints, img2->data(),
                            this_keypoints, matches, out_img, Scalar(0, 0, 255),
                            Scalar(255, 0, 0));

                const string output = fmt::format(*output_pattern, idx);
                imwrite(output, out_img);
            }
        } catch (...) {
            std::cerr << sens_loc::util::err{}
                      << "Could not initialize data for idx: " << idx << "\n";
            return;
        }
    }

    void postprocess(const optional<string>& stat_file,
                     const optional<string>& matched_distance_histo) {
        lock_guard guard{*distance_mutex};
        if (distances->empty())
            return;

        sort(begin(*distances), end(*distances));
        const auto                   dist_bins = 25;
        sens_loc::analysis::distance distance_stat{*distances, dist_bins};

        if (stat_file) {
            cv::FileStorage stat_out{*stat_file,
                                     cv::FileStorage::WRITE |
                                         cv::FileStorage::FORMAT_YAML};
            stat_out.writeComment(
                "The following values contain the results of the statistical "
                "analysis for descriptor distance to the closest descriptor "
                "after matching");
            write(stat_out, "match_distance", distance_stat.get_statistic());
            stat_out.release();
        } else {
            cout << "==== Match Distances\n"
                 << "total count:    " << *total_descriptors << "\n"
                 << "matched count:  " << distances->size() << "\n"
                 << "matched/total:  "
                 << static_cast<double>(distances->size()) /
                        static_cast<double>(*total_descriptors)
                 << "\n"
                 << "min:            " << distance_stat.min() << "\n"
                 << "max:            " << distance_stat.max() << "\n"
                 << "median:         " << distance_stat.median() << "\n"
                 << "mean:           " << distance_stat.mean() << "\n"
                 << "Variance:       " << distance_stat.variance() << "\n"
                 << "StdDev:         " << distance_stat.stddev() << "\n"
                 << "Skewness:       " << distance_stat.skewness() << "\n";
        }
        if (matched_distance_histo) {
            std::ofstream gnuplot_data{*matched_distance_histo};
            gnuplot_data << sens_loc::io::to_gnuplot(distance_stat.histogram())
                         << std::endl;
        } else {
            cout << distance_stat.histogram() << "\n";
        }
    }

  private:
    not_null<mutex*>         distance_mutex;
    not_null<vector<float>*> distances;
    not_null<uint64_t*>      total_descriptors;
    Ptr<BFMatcher>           matcher;
    string_view              input_pattern;
    optional<string_view>    output_pattern;
    optional<string_view>    original_images;
};
}  // namespace

namespace sens_loc::apps {
int analyze_matching(util::processing_input       in,
                     NormTypes                    norm_to_use,
                     bool                         crosscheck,
                     const optional<string>&      stat_file,
                     const optional<string>&      matched_distance_histo,
                     const optional<string_view>& output_pattern,
                     const optional<string_view>& original_files) {
    Expects(in.start < in.end && "Matching requires at least 2 images");

    mutex         distance_mutex;
    vector<float> global_minimal_distances;
    uint64_t      total_descriptors;

    using visitor   = statistic_visitor<matching, required_data::descriptors>;
    auto analysis_v = visitor{/*input_pattern=*/in.input_pattern,
                              /*distance_mutex=*/not_null{&distance_mutex},
                              /*distances=*/not_null{&global_minimal_distances},
                              /*descriptor_count=*/not_null{&total_descriptors},
                              /*norm_to_use=*/norm_to_use,
                              /*crosscheck=*/crosscheck,
                              /*input_pattern=*/in.input_pattern,
                              /*output_pattern=*/output_pattern,
                              /*original_files=*/original_files};

    auto f = parallel_visitation(
        in.start + 1,  // Because two consecutive images are matched, the first
                       // index is skipped. This requires "backwards" matching.
        in.end, analysis_v);

    f.postprocess(stat_file, matched_distance_histo);

    return !global_minimal_distances.empty() ? 0 : 1;
}
}  // namespace sens_loc::apps
