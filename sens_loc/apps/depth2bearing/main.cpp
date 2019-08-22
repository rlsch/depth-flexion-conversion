#include <CLI/CLI.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <rang.hpp>
#include <sens_loc/io/image.h>
#include <sens_loc/io/intrinsics.h>
#include <sens_loc/util/console.h>

using namespace sens_loc;

int main(int argc, char **argv) {
    CLI::App app{"Convert depth images to pointclouds"};

    std::string calibration_file;
    app.add_option("-c,--calibration", calibration_file,
                   "File that contains calibration parameters for the camera")
        ->required()
        ->check(CLI::ExistingFile);

    std::string input_file{"/home/jonas/owncloud/Freiberg/Masterarbeit/"
                           "full_data_sets/lehrpfad/raw_data/data196.jpg"};
    app.add_option("-i,--input", input_file, "Input depth map")
        ->required()
        ->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);

    cv::Mat depth_image;
    {
        std::optional<cv::Mat> image = io::load_image(input_file);
        if (!image) {
            std::cerr << util::err{};
            std::cerr << "Could not load image \"" << rang::style::bold
                      << input_file << rang::style::reset << "\"!\n";
            std::exit(1);
        }
        std::swap(*image, depth_image);
    }

    io::pinhole_parameters intrinsic;
    {
        std::ifstream calibration_fstream{calibration_file};
        std::optional<io::pinhole_parameters> calibration =
            io::load_pinhole_intrinsic(calibration_fstream);
        if (!calibration) {
            std::cerr << util::err{};
            std::cerr << "Could not intrinsic calibration \""
                      << rang::style::bold << calibration_file
                      << rang::style::reset << "\"!\n";
            std::exit(1);
        }
        std::swap(*calibration, intrinsic);
    }

    return 0;
}
