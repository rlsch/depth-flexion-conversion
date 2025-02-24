#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <sens_loc/camera_models/pinhole.h>
#include <sens_loc/conversion/util.h>

using namespace sens_loc;
using namespace sens_loc::conversion;
using doctest::Approx;

TEST_CASE("depth orthografic to euclidian") {
    camera_models::pinhole<double> p = {
        10, 10, 10., 10., 5., 5.,
    };

    using namespace detail;
    SUBCASE("zero arg") {
        const int    d = 0;
        const double e = orthografic_to_euclidian<double, int>({5, 5}, d, p);
        REQUIRE(e == 0.);
    }
    SUBCASE("directly over center") {
        const int    d = 20;
        const double e = orthografic_to_euclidian<double, int>({5, 5}, d, p);
        REQUIRE(e == Approx(20.));
    }
    SUBCASE("not center") {
        const int    d = 20;
        const double e = orthografic_to_euclidian<double, int>({8, 8}, d, p);
        MESSAGE(e);
        REQUIRE(e > 20.);
    }
}

TEST_CASE("Scaling of bearing angle") {
    using namespace detail;

    SUBCASE("scale double to uchar8") {
        const double min_angle = 0.;
        const double max_angle = math::pi<double>;
        const double mid_angle = math::pi<double> / 2.;

        auto [scale, offset] = scaling_factor<double, uchar>(max_angle);

        REQUIRE(offset == 0.);
        REQUIRE(gsl::narrow_cast<uchar>(max_angle * scale) == 254);
        REQUIRE(gsl::narrow_cast<uchar>(min_angle * scale) == 0);
        REQUIRE(gsl::narrow_cast<uchar>(mid_angle * scale) == 127);
    }
    SUBCASE("scale double to uchar16") {
        const double min_angle = 0.;
        const double max_angle = math::pi<double>;
        const double mid_angle = math::pi<double> / 2.;

        auto [scale, offset] = scaling_factor<double, ushort>(max_angle);

        REQUIRE(offset == 0.);
        REQUIRE(gsl::narrow_cast<ushort>(max_angle * scale) == 65'535);
        REQUIRE(gsl::narrow_cast<ushort>(min_angle * scale) == 0);
        REQUIRE(gsl::narrow_cast<ushort>(mid_angle * scale) == 65'535 / 2);
    }
    SUBCASE("scale double to char8") {
        const double min_angle = 0.;
        const double max_angle = math::pi<double>;
        const double mid_angle = math::pi<double> / 2.;

        auto [scale, offset] = scaling_factor<double, schar>(max_angle);

        MESSAGE("Max Angle: " << max_angle);
        MESSAGE("Scale is: " << scale);
        MESSAGE("Offset is: " << offset);
        REQUIRE(offset == -128);
        REQUIRE(gsl::narrow_cast<schar>(max_angle * scale + offset) == 126);
        REQUIRE(gsl::narrow_cast<schar>(min_angle * scale + offset) == -128);
        REQUIRE(gsl::narrow_cast<schar>(mid_angle * scale + offset) == 0);
    }
    SUBCASE("scale double to char16") {
        const double min_angle = 0.;
        const double max_angle = math::pi<double>;
        const double mid_angle = math::pi<double> / 2.;

        auto [scale, offset] = scaling_factor<double, short>(max_angle);

        REQUIRE(offset == -32768.);
        REQUIRE(gsl::narrow_cast<short>(max_angle * scale + offset) == 32'767);
        REQUIRE(gsl::narrow_cast<short>(min_angle * scale + offset) == -32'768);
        REQUIRE(gsl::narrow_cast<short>(mid_angle * scale + offset) == 0);
    }
}
