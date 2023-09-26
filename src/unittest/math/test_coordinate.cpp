#include <doctest/doctest.h>
#include <sens_loc/math/coordinate.h>

using doctest::Approx;
using namespace sens_loc::math;

TEST_CASE("world coordinate") {
    SUBCASE("default construction") {
        coordinate<double, frame::world> p;
        REQUIRE(p.U() == 0.);
        REQUIRE(p.V() == 0.);
        REQUIRE(p.W() == 0.);
    }
    SUBCASE("init Construction") {
        coordinate<double, frame::world> p{10., 2., -2.};
        REQUIRE(p.U() == 10.);
        REQUIRE(p.V() == 2.);
        REQUIRE(p.W() == -2.);
    }
    SUBCASE("assignment") {
        coordinate<double, frame::world> p1{10., 2., -2.};
        coordinate<double, frame::world> p2{11., 2., -2.};
        p1 = p2;
        REQUIRE(p1.U() == 11.);
        REQUIRE(p1.V() == 2.);
        REQUIRE(p1.W() == -2.);
    }
    SUBCASE("norm for real coordinates") {
        coordinate<double, frame::world> p1;
        REQUIRE(p1.norm() == 0.);

        coordinate<double, frame::world> p2(3., 4., 5.);
        REQUIRE(p2.norm() == std::sqrt(50.));
    }
    SUBCASE("dot product") {
        coordinate<double, frame::world> p1, p2;
        REQUIRE(p1.dot(p2) == 0.);

        coordinate<double, frame::world> p3(3., 4., 5.), p4(3., 4., 5.);
        REQUIRE(p3.dot(p4) == 50.);
    }
    SUBCASE("difference") {
        coordinate<double, frame::world> p1{10., 2., -5.};
        coordinate<double, frame::world> p2{11., 10., -2.};

        coordinate<double, frame::world> diff = p2 - p1;

        REQUIRE(diff.U() == Approx(1.0));
        REQUIRE(diff.V() == Approx(8.0));
        REQUIRE(diff.W() == Approx(3.0));
    }
    SUBCASE("sum") {
        coordinate<double, frame::world> p1{10., 2., -5.};
        coordinate<double, frame::world> p2{11., 10., -2.};

        coordinate<double, frame::world> sum = p2 + p1;

        REQUIRE(sum.U() == Approx(21.0));
        REQUIRE(sum.V() == Approx(12.0));
        REQUIRE(sum.W() == Approx(-7.0));
    }
    SUBCASE("multiplication") {
        coordinate<double, frame::world> p1{10., -2., -5.};
        coordinate<double, frame::world> p2{11., 10., -2.};

        coordinate<double, frame::world> res1 = p1 * 10.;
        coordinate<double, frame::world> res2 = p2 * -5.;

        REQUIRE(res1.U() == Approx(100.0));
        REQUIRE(res1.V() == Approx(-20.0));
        REQUIRE(res1.W() == Approx(-50.0));
        REQUIRE(res2.U() == Approx(-55.0));
        REQUIRE(res2.V() == Approx(-50.0));
        REQUIRE(res2.W() == Approx(10.0));
    }
}

TEST_CASE("pixel coordinate") {
    SUBCASE("default construction") {
        coordinate<int, frame::pixel> p;
        REQUIRE(p.u() == 0);
        REQUIRE(p.v() == 0);
    }
    SUBCASE("init Construction") {
        coordinate<int, frame::pixel> p{100, 200};
        REQUIRE(p.u() == 100);
        REQUIRE(p.v() == 200);
    }
    SUBCASE("assignment") {
        coordinate<int, frame::pixel> p1{100, 200};
        coordinate<int, frame::pixel> p2{112, 22};
        p1 = p2;
        REQUIRE(p1.u() == 112);
        REQUIRE(p1.v() == 22);
    }
}

TEST_CASE("Scale spherical coordinates") {
    SUBCASE("x coord works") {
        sphere_coord<float> p0{1.F, 0.0F, 0.0F};
        camera_coord<float> c0{10.0F * p0};

        REQUIRE(c0.norm() == Approx(10.0F));
        REQUIRE(c0.X() == Approx(10.0F));
        REQUIRE(c0.Y() == Approx(0.0F));
        REQUIRE(c0.Z() == Approx(0.0F));
    }

    SUBCASE("y coord works") {
        sphere_coord<float> p0{0.F, 1.0F, 0.0F};
        camera_coord<float> c0{10.0F * p0};

        REQUIRE(c0.norm() == Approx(10.0F));
        REQUIRE(c0.X() == Approx(0.0F));
        REQUIRE(c0.Y() == Approx(10.0F));
        REQUIRE(c0.Z() == Approx(0.0F));
    }
    SUBCASE("z coord works") {
        sphere_coord<float> p0{0.F, 0.0F, 1.0F};
        camera_coord<float> c0{10.0F * p0};

        REQUIRE(c0.norm() == Approx(10.0F));
        REQUIRE(c0.X() == Approx(0.0F));
        REQUIRE(c0.Y() == Approx(0.0F));
        REQUIRE(c0.Z() == Approx(10.0F));
    }
}
TEST_CASE("Scale camera coordinates") {
    SUBCASE("x coord works") {
        camera_coord<float> p0{1.F, 0.0F, 0.0F};
        camera_coord<float> c0{10.0F * p0};

        REQUIRE(c0.norm() == Approx(10.0F));
        REQUIRE(c0.X() == Approx(10.0F));
        REQUIRE(c0.Y() == Approx(0.0F));
        REQUIRE(c0.Z() == Approx(0.0F));
    }

    SUBCASE("y coord works") {
        camera_coord<float> p0{0.F, 1.0F, 0.0F};
        camera_coord<float> c0{10.0F * p0};

        REQUIRE(c0.norm() == Approx(10.0F));
        REQUIRE(c0.X() == Approx(0.0F));
        REQUIRE(c0.Y() == Approx(10.0F));
        REQUIRE(c0.Z() == Approx(0.0F));
    }
    SUBCASE("z coord works") {
        camera_coord<float> p0{0.F, 0.0F, 1.0F};
        camera_coord<float> c0{10.0F * p0};

        REQUIRE(c0.norm() == Approx(10.0F));
        REQUIRE(c0.X() == Approx(0.0F));
        REQUIRE(c0.Y() == Approx(0.0F));
        REQUIRE(c0.Z() == Approx(10.0F));
    }
}
