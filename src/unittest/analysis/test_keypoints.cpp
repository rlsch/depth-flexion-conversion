#include <doctest/doctest.h>
#include <sens_loc/analysis/keypoints.h>
#include <vector>

using namespace std;
using namespace cv;
using namespace sens_loc;
using namespace sens_loc::analysis;

const vector<KeyPoint> pts{
    {10.0F, 1.0F, 5.0F}, {5.0F, 5.0F, 5.0F},  {24.0F, 8.0F, 5.0F},
    {12.0F, 4.0F, 5.0F}, {30.0F, 3.0F, 5.0F}, {15.0F, 10.0F, 5.0F},
    {3.0F, 2.0F, 5.0F},
};

TEST_CASE("normal keypoints") {
    SUBCASE("combined configuration functions") {
        keypoints kp_ana;
        kp_ana.configure_image_dimension(35U, 15U);
        kp_ana.configure_size(3, "size distribution");
        kp_ana.configure_response(2, "response distribution");
        kp_ana.configure_distribution(2);
        kp_ana.configure_distribution("width dist", "height dist");

        SUBCASE("disable all analysis") {
            kp_ana.analyze(pts, /*distribution=*/false, /*size=*/false,
                           /*response=*/false);
            CHECK(kp_ana.response().count == 0UL);
            CHECK(kp_ana.response().min == 0.0F);
            CHECK(kp_ana.response().max == 0.0F);
            CHECK(kp_ana.response_histo().size() == 0UL);

            CHECK(kp_ana.size().count == 0UL);
            CHECK(kp_ana.size().min == 0.0F);
            CHECK(kp_ana.size().max == 0.0F);
            CHECK(kp_ana.size_histo().size() == 0UL);

            CHECK(kp_ana.distribution().size() == 0UL);
        }

        SUBCASE("calculate only distribution") {
            kp_ana.enable_response_histo(false);
            kp_ana.enable_size_histo(false);
            kp_ana.configure_distribution(3U, 2U);
            kp_ana.analyze(pts, /*distribution=*/true, /*size=*/false,
                           /*response=*/false);

            CHECK(kp_ana.response().count == 0UL);
            CHECK(kp_ana.response().min == 0.0F);
            CHECK(kp_ana.response().max == 0.0F);
            CHECK(kp_ana.response_histo().size() == 0UL);

            CHECK(kp_ana.size().count == 0UL);
            CHECK(kp_ana.size().min == 0.0F);
            CHECK(kp_ana.size().max == 0.0F);
            CHECK(kp_ana.size_histo().size() == 0UL);

            CHECK(kp_ana.distribution().rank() == 2UL);
            CHECK(kp_ana.distribution().axis(0).size() == 3UL);
            CHECK(kp_ana.distribution().axis(1).size() == 2UL);
            CHECK(kp_ana.distribution().at(0, 0) == 3UL);
        }

        SUBCASE("no distribution and no histos") {
            kp_ana.enable_response_histo(false);
            kp_ana.enable_size_histo(false);
            kp_ana.configure_distribution(3U, 2U);
            kp_ana.analyze(pts, /*distribution=*/false, /*size=*/true,
                           /*response=*/true);

            CHECK(kp_ana.response().count == 7UL);
            CHECK(kp_ana.response().min == 0.0F);
            CHECK(kp_ana.response().max == 0.0F);
            CHECK(kp_ana.response_histo().size() == 0UL);

            CHECK(kp_ana.size().count == 7UL);
            CHECK(kp_ana.size().min == 5.0F);
            CHECK(kp_ana.size().max == 5.0F);
            CHECK(kp_ana.size_histo().size() == 0UL);

            CHECK(kp_ana.distribution().rank() == 2UL);
            CHECK(kp_ana.distribution().axis(0).size() == 0UL);
            CHECK(kp_ana.distribution().axis(1).size() == 0UL);
        }

        SUBCASE("analyse everything") {
            kp_ana.enable_response_histo(true);
            kp_ana.enable_size_histo(true);
            kp_ana.configure_distribution(3U, 2U);
            kp_ana.analyze(pts);

            CHECK(kp_ana.response().count == 7UL);
            CHECK(kp_ana.response().min == 0.0F);
            CHECK(kp_ana.response().max == 0.0F);
            CHECK(kp_ana.response_histo().size() == 2UL);
            CHECK(kp_ana.response_histo().at(1) == 7UL);
            CHECK(kp_ana.response_histo().axis().metadata() ==
                  "response distribution");

            CHECK(kp_ana.size().count == 7UL);
            CHECK(kp_ana.size().min == 5.0F);
            CHECK(kp_ana.size().max == 5.0F);
            CHECK(kp_ana.size_histo().size() == 3UL);
            CHECK(kp_ana.size_histo().at(1) == 7UL);
            CHECK(kp_ana.size_histo().axis().metadata() == "size distribution");

            CHECK(kp_ana.distribution().rank() == 2UL);
            CHECK(kp_ana.distribution().at(0, 0) == 3UL);
            CHECK(kp_ana.distribution().axis(0).size() == 3UL);
            CHECK(kp_ana.distribution().axis(0).metadata() == "width dist");
            CHECK(kp_ana.distribution().axis(1).size() == 2UL);
            CHECK(kp_ana.distribution().axis(1).metadata() == "height dist");
        }
    }
}

TEST_CASE("No keypoints") {
    keypoints kp_ana;
    kp_ana.configure_image_dimension(35U, 15U);
    kp_ana.configure_size(3, "size distribution");
    kp_ana.configure_response(2, "response distribution");
    kp_ana.configure_distribution(2);
    kp_ana.configure_distribution("width dist", "height dist");
    kp_ana.analyze({});

    CHECK(kp_ana.response().count == 0UL);
    CHECK(kp_ana.response().min == 0.0F);
    CHECK(kp_ana.response().max == 0.0F);
    CHECK(kp_ana.size().count == 0UL);
    CHECK(kp_ana.size().min == 0.0F);
    CHECK(kp_ana.size().max == 0.0F);
    CHECK(kp_ana.distribution().rank() == 2UL);
}

TEST_CASE("write with filestorage") {
    keypoints kp_ana;
    SUBCASE("empty") {
        cv::FileStorage kp_statistic{
            "keypoint.stat", cv::FileStorage::MEMORY | cv::FileStorage::WRITE |
                                 cv::FileStorage::FORMAT_YAML};
        write(kp_statistic, "empty", kp_ana);
        std::string written = kp_statistic.releaseAndGetString();
        REQUIRE(written == "%YAML:1.0\n"
                           "---\n"
                           "empty:\n"
                           "   response:\n"
                           "      count: 0\n"
                           "      min: 0.\n"
                           "      max: 0.\n"
                           "      median: 0.\n"
                           "      mean: 0.\n"
                           "      variance: 0.\n"
                           "      stddev: 0.\n"
                           "      skewness: 0.\n"
                           "   size:\n"
                           "      count: 0\n"
                           "      min: 0.\n"
                           "      max: 0.\n"
                           "      median: 0.\n"
                           "      mean: 0.\n"
                           "      variance: 0.\n"
                           "      stddev: 0.\n"
                           "      skewness: 0.\n");
    }

    SUBCASE("normal analysis of everything") {
        kp_ana.enable_response_histo(true);
        kp_ana.enable_size_histo(true);
        kp_ana.configure_distribution(3U, 2U);
        kp_ana.analyze(pts);

        cv::FileStorage kp_statistic{
            "keypoint.stat", cv::FileStorage::MEMORY | cv::FileStorage::WRITE |
                                 cv::FileStorage::FORMAT_YAML};
        write(kp_statistic, "everything", kp_ana);
        std::string written = kp_statistic.releaseAndGetString();
        REQUIRE(written == "%YAML:1.0\n"
                           "---\n"
                           "everything:\n"
                           "   response:\n"
                           "      count: 7\n"
                           "      min: 0.\n"
                           "      max: 0.\n"
                           "      median: 0.\n"
                           "      mean: 0.\n"
                           "      variance: 0.\n"
                           "      stddev: 0.\n"
                           "      skewness: .Nan\n"
                           "   size:\n"
                           "      count: 7\n"
                           "      min: 5.\n"
                           "      max: 5.\n"
                           "      median: 5.\n"
                           "      mean: 5.\n"
                           "      variance: 0.\n"
                           "      stddev: 1.3999999500811100e-03\n"
                           "      skewness: -7.9648510742187500e+03\n");
    }
}
