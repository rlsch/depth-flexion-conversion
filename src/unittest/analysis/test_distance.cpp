#include <doctest/doctest.h>
#include <opencv2/core/persistence.hpp>
#include <sens_loc/analysis/distance.h>
#include <vector>

using namespace std;
using namespace sens_loc;

const vector<float> distances{10.0F, 0.0F, 2.0F, 5.0F, -10.F, -2.0F, 1.5F};

TEST_CASE("normal analysis") {
    SUBCASE("Default construction and then analysis") {
        analysis::distance d_ana;
        SUBCASE("with histogram") {
            d_ana.analyze(distances);
            CHECK(d_ana.count() == distances.size());
            CHECK(d_ana.min() == -10.0F);
            CHECK(d_ana.max() == 10.0F);
            // The median is estimated through running quantiles or something
            // like this.
            // This is not so accurate with a small sample size.
            CHECK(d_ana.median() == doctest::Approx(0.3333333F));
            CHECK(d_ana.mean() ==
                  doctest::Approx((5.0F + 1.5F) / distances.size()));
            CHECK(d_ana.variance() != 0.0F);
            CHECK(d_ana.stddev() * d_ana.stddev() ==
                  doctest::Approx(d_ana.variance()));
            CHECK(d_ana.skewness() != 0.0F);

            CHECK(d_ana.histogram().size() == 50UL);
            CHECK(d_ana.histogram().at(0) == 1UL);
            CHECK(d_ana.histogram().axis().metadata() == "distance");
        }

        SUBCASE("without histogram") {
            d_ana.analyze(distances, /*histo=*/false);
            CHECK(d_ana.count() == distances.size());
            CHECK(d_ana.min() == -10.0F);
            CHECK(d_ana.max() == 10.0F);
            // The median is estimated through running quantiles or something
            // like this.
            // This is not so accurate with a small sample size.
            CHECK(d_ana.median() == doctest::Approx(0.3333333F));
            CHECK(d_ana.mean() ==
                  doctest::Approx((5.0F + 1.5F) / distances.size()));
            CHECK(d_ana.variance() != 0.0F);
            CHECK(d_ana.stddev() * d_ana.stddev() ==
                  doctest::Approx(d_ana.variance()));
            CHECK(d_ana.skewness() != 0.0F);

            CHECK(d_ana.histogram().size() == 0UL);
        }
    }

    SUBCASE("Analyze through constructor") {
        analysis::distance d_ana{distances, 4U, "Quantity"};

        CHECK(d_ana.count() == distances.size());
        CHECK(d_ana.min() == -10.0F);
        CHECK(d_ana.max() == 10.0F);
        // The median is estimated through running quantiles or something
        // like this.
        // This is not so accurate with a small sample size.
        CHECK(d_ana.median() == doctest::Approx(0.3333333F));
        CHECK(d_ana.mean() ==
              doctest::Approx((5.0F + 1.5F) / distances.size()));
        CHECK(d_ana.variance() != 0.0F);
        CHECK(d_ana.stddev() * d_ana.stddev() ==
              doctest::Approx(d_ana.variance()));
        CHECK(d_ana.skewness() != 0.0F);

        CHECK(d_ana.histogram().size() == 4UL);
        CHECK(d_ana.histogram().at(0) == 1UL);
        CHECK(d_ana.histogram().axis().metadata() == "Quantity");
    }

    SUBCASE("Empty Data") {
        analysis::distance d_ana{{}, 4U, "Quantity"};
        CHECK(d_ana.count() == 0UL);
    }
}

TEST_CASE("write with filestorage") {
    SUBCASE("Write Empty Statistic") {
        analysis::distance d_ana{{}, 4U, "Quantity"};

        cv::FileStorage kp_statistic{
            "keypoint.stat", cv::FileStorage::MEMORY | cv::FileStorage::WRITE |
                                 cv::FileStorage::FORMAT_YAML};
        analysis::write(kp_statistic, "test_empty", d_ana.get_statistic());

        std::string written = kp_statistic.releaseAndGetString();
        REQUIRE(written == "%YAML:1.0\n"
                           "---\n"
                           "test_empty:\n"
                           "   count: 0\n"
                           "   min: 0.\n"
                           "   max: 0.\n"
                           "   median: 0.\n"
                           "   mean: 0.\n"
                           "   variance: 0.\n"
                           "   stddev: 0.\n"
                           "   skewness: 0.\n");
    }

    SUBCASE("Basic Analysis") {
        analysis::distance d_ana{distances, 4U, "Quantity"};
        cv::FileStorage    kp_statistic{
            "keypoint.stat", cv::FileStorage::MEMORY | cv::FileStorage::WRITE |
                                 cv::FileStorage::FORMAT_YAML};
        analysis::write(kp_statistic, "test_basic", d_ana.get_statistic());
        std::string written = kp_statistic.releaseAndGetString();
        REQUIRE(written == "%YAML:1.0\n"
                           "---\n"
                           "test_basic:\n"
                           "   count: 7\n"
                           "   min: -10.\n"
                           "   max: 10.\n"
                           "   median: 3.3333349227905273e-01\n"
                           "   mean: 9.2857146263122559e-01\n"
                           "   variance: 3.2744899749755859e+01\n"
                           "   stddev: 5.7223157882690430e+00\n"
                           "   skewness: -3.9321494102478027e-01\n");
    }
}
