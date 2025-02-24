#include <doctest/doctest.h>
#include <sens_loc/math/image.h>

using namespace sens_loc::math;

TEST_CASE("image access") {
    SUBCASE("default constructor") { image<unsigned char> def; }

    cv::Mat m(20, 20, CV_16U);
    m = 0;
    // Copy construction with cv::Mat
    image<ushort> i(m);

    SUBCASE("dimensions") {
        CHECK(i.w() == 20);
        CHECK(i.h() == 20);
    }

    SUBCASE("read access") { REQUIRE(i.at(pixel_coord<int>(10, 10)) == 0); }
    SUBCASE("write access") {
        i.at(pixel_coord<int>(10, 10)) = 10;
        REQUIRE(i.at(pixel_coord<int>(10, 10)) == 10);
    }

    i.at(pixel_coord<int>(11, 11)) = 42;
    SUBCASE("copy construction with image") {
        image<ushort> f(i);
        REQUIRE(f.at(pixel_coord<int>(11, 11)) == 42);
        REQUIRE(f.at(pixel_coord<int>(0, 0)) == 0);
    }


    SUBCASE("move construction with image") {
        image<ushort> my_i(i);
        image<ushort> f(std::move(my_i));
        REQUIRE(f.at(pixel_coord<int>(11, 11)) == 42);
        REQUIRE(f.at(pixel_coord<int>(0, 0)) == 0);
    }
    SUBCASE("move construction with matrix") {
        cv::Mat m_assign(20, 20, CV_16U);
        m_assign                    = 0;
        m_assign.at<ushort>(11, 11) = 69;
        image<ushort> f(std::move(m_assign));
        REQUIRE(f.at(pixel_coord<int>(11, 11)) == 69);
        REQUIRE(f.at(pixel_coord<int>(0, 0)) == 0);
    }


    SUBCASE("copy assignment with image") {
        image<ushort> f;
        f = i;
        REQUIRE(f.at(pixel_coord<int>(11, 11)) == 42);
        REQUIRE(f.at(pixel_coord<int>(0, 0)) == 0);
    }
    SUBCASE("copy assignment with matrix") {
        image<ushort> f;
        cv::Mat       m_assign(20, 20, CV_16U);
        m_assign                    = 0;
        m_assign.at<ushort>(11, 11) = 69;
        f                           = m_assign;
        REQUIRE(f.at(pixel_coord<int>(11, 11)) == 69);
        REQUIRE(f.at(pixel_coord<int>(0, 0)) == 0);
    }


    SUBCASE("move assignment with image") {
        image<ushort> my_i(i);
        image<ushort> f;
        f = std::move(my_i);
        REQUIRE(f.at(pixel_coord<int>(11, 11)) == 42);
        REQUIRE(f.at(pixel_coord<int>(0, 0)) == 0);
    }
    SUBCASE("move assignment with matrix") {
        image<ushort> f;
        cv::Mat       m_assign(20, 20, CV_16U);
        m_assign                    = 0;
        m_assign.at<ushort>(11, 11) = 69;
        f                           = std::move(m_assign);
        REQUIRE(f.at(pixel_coord<int>(11, 11)) == 69);
        REQUIRE(f.at(pixel_coord<int>(0, 0)) == 0);
    }

    SUBCASE("underlying data") { REQUIRE(i.data().at<ushort>(10, 10) == 0); }
}

TEST_CASE("image convert") {
    cv::Mat m(20, 20, CV_32F);
    m = 42.5F;

    SUBCASE("different types") {
        image<float>         foo(m);
        image<unsigned char> bar = convert<unsigned char>(foo);
        REQUIRE(bar.at({5, 5}) == 42U);
    }
    SUBCASE("same types") {
        image<float> foo(m);
        image<float> bar = convert<float>(foo);
        REQUIRE(bar.at({5, 5}) == 42.5F);
    }
}

TEST_CASE("get_cv_type") {
    using namespace detail;
    REQUIRE(get_opencv_type<uchar>() == CV_8U);
    REQUIRE(get_opencv_type<ushort>() == CV_16U);
    REQUIRE(get_opencv_type<schar>() == CV_8S);
    REQUIRE(get_opencv_type<short>() == CV_16S);
    REQUIRE(get_opencv_type<float>() == CV_32F);
    REQUIRE(get_opencv_type<double>() == CV_64F);
    REQUIRE(get_opencv_type<long long>() == -1);
}
