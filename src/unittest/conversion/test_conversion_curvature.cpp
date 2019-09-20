#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <sens_loc/conversion/depth_to_curvature.h>
#include <sens_loc/conversion/depth_to_laserscan.h>
#include <sens_loc/io/image.h>
#include <sens_loc/util/correctness_util.h>

using namespace sens_loc;
constexpr camera_models::pinhole p = {
    .w  = 960,
    .h  = 540,
    .fx = 519.226,
    .fy = 479.462,
    .cx = 522.23,
    .cy = 272.737,
};

TEST_CASE("gaussian curvature") {
    std::optional<cv::Mat> depth_image =
        io::load_image("conversion/data0-depth.png", cv::IMREAD_UNCHANGED);
    REQUIRE(depth_image);
    REQUIRE((*depth_image).type() == CV_16U);

    std::optional<cv::Mat> ref_image =
        io::load_image("conversion/gauss-reference.png", cv::IMREAD_UNCHANGED);
    // REQUIRE(ref_image);

    cv::Mat laser_double =
        conversion::depth_to_laserscan<double, ushort>(*depth_image, p);

    const auto gauss = conversion::depth_to_gaussian_curvature<double, double>(
        laser_double, p);
    const auto converted = conversion::curvature_to_image<double, ushort>(
        gauss, *depth_image, -20., 20.);
    cv::imwrite("conversion/test_gauss.png", converted);

    // REQUIRE(util::average_pixel_error(gauss, *ref_image) < 0.5);
}

TEST_CASE("mean curvature") {
    std::optional<cv::Mat> depth_image =
        io::load_image("conversion/data0-depth.png", cv::IMREAD_UNCHANGED);
    REQUIRE(depth_image);
    REQUIRE((*depth_image).type() == CV_16U);

    std::optional<cv::Mat> ref_image =
        io::load_image("conversion/mean-reference.png", cv::IMREAD_UNCHANGED);
    // REQUIRE(ref_image);

    cv::Mat laser_double =
        conversion::depth_to_laserscan<double, ushort>(*depth_image, p);

    const auto mean =
        conversion::depth_to_mean_curvature<double, double>(laser_double, p);
    const auto converted = conversion::curvature_to_image<double, ushort>(
        mean, *depth_image, -20., 20.);
    cv::imwrite("conversion/test_mean.png", converted);

    // REQUIRE(util::average_pixel_error(mean, *ref_image) < 0.5);
}
