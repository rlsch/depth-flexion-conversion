#include <cmath>
#include <doctest/doctest.h>
#include <sens_loc/camera_models/pinhole.h>
#include <sens_loc/camera_models/utility.h>
#include <sens_loc/math/angle_conversion.h>

using namespace sens_loc::camera_models;
using namespace sens_loc::math;
using namespace std;
using doctest::Approx;

TEST_CASE("Calculate angular resolution") {
    pinhole<float> p = {
        960, 540, 519.226, 479.462, 522.23, 272.737,
    };

    // Angles must be symmetric.
    CHECK(phi(p, {0, 1}, {1, 1}) == phi(p, {1, 1}, {0, 1}));
    CHECK(phi(p, {1, 0}, {1, 1}) == phi(p, {1, 1}, {1, 0}));
    CHECK(phi(p, {1, 1}, {0, 0}) == phi(p, {0, 0}, {1, 1}));
    CHECK(phi(p, {1, 0}, {0, 1}) == phi(p, {0, 1}, {1, 0}));

    // Diagonals create a bigger angle.
    CHECK(phi(p, {0, 0}, {1, 1}) > phi(p, {0, 0}, {1, 0}));
    CHECK(phi(p, {0, 0}, {1, 1}) > phi(p, {0, 0}, {0, 1}));

    // Angular resolution gets finer in outer parts of image.
    CHECK(phi(p, {0, 0}, {1, 0}) < phi(p, {480, 0}, {481, 0}));
    CHECK(phi(p, {900, 0}, {901, 0}) < phi(p, {480, 0}, {481, 0}));
}

TEST_CASE("Project pixels to sphere") {
    pinhole<double> p = {
        960, 540, 519.226, 479.462, 522.23, 272.737,
    };
    SUBCASE("Dimensions") {
        CHECK(p.w() == 960);
        CHECK(p.h() == 540);
    }
    SUBCASE("Coordinate Transformations") {
        const pixel_coord<double>  pixel(50, 50);
        const image_coord<double>  img       = p.transform_to_image(pixel);
        const sphere_coord<double> P_s_pixel = p.pixel_to_sphere(pixel);
        const sphere_coord<double> P_s_image = p.image_to_sphere(img);

        // Both coordinates needed to be identical. This can be checked
        // with 'v \cdot v == abs(v) * abs(v)'.
        CHECK(P_s_pixel.norm() * P_s_image.norm() == P_s_pixel.dot(P_s_image));
    }
    SUBCASE("horizontal") {
        auto p0 = p.pixel_to_sphere({1, 1});
        CHECK(p0.norm() == Approx(1.));

        auto p1 = p.pixel_to_sphere({2, 1});
        CHECK(p1.norm() == Approx(1.));

        const auto cos_phi = p0.dot(p1);
        const auto angle   = std::acos(cos_phi);
        MESSAGE("horiz: acos(phi) == " << rad_to_deg(angle) << "°");
    }
    SUBCASE("vertical") {
        auto p0 = p.pixel_to_sphere({1, 1});
        CHECK(p0.norm() == Approx(1.));

        auto p1 = p.pixel_to_sphere({1, 2});
        CHECK(p1.norm() == Approx(1.));

        const auto cos_phi = p0.dot(p1);
        const auto angle   = std::acos(cos_phi);
        MESSAGE("vert: acos(phi) == " << rad_to_deg(angle) << "°");
    }

    SUBCASE("diagonal") {
        auto p0 = p.pixel_to_sphere({1, 1});
        CHECK(p0.norm() == Approx(1.));

        auto p1 = p.pixel_to_sphere({2, 2});
        CHECK(p1.norm() == Approx(1.));

        const auto cos_phi = p0.dot(p1);
        const auto angle   = std::acos(cos_phi);
        MESSAGE("diag: acos(phi) == " << rad_to_deg(angle) << "°");
    }

    SUBCASE("antidiagonal") {
        auto p0 = p.pixel_to_sphere({1, 2});
        CHECK(p0.norm() == Approx(1.));

        auto p1 = p.pixel_to_sphere({2, 1});
        CHECK(p1.norm() == Approx(1.));

        const auto cos_phi = p0.dot(p1);
        const auto angle   = std::acos(cos_phi);
        MESSAGE("antidiag: acos(phi) == " << rad_to_deg(angle) << "°");
    }
}
