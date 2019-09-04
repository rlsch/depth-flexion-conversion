#include <CLI/CLI.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <rang.hpp>
#include <sens_loc/conversion/depth_to_bearing.h>
#include <sens_loc/conversion/depth_to_laserscan.h>
#include <sens_loc/io/image.h>
#include <sens_loc/io/intrinsics.h>
#include <sens_loc/util/console.h>
#include <sens_loc/version.h>
#include <taskflow/taskflow.hpp>
#include <vector>

using namespace sens_loc;
using namespace std;

namespace detail {
// clang-format off
template <conversion::direction Direction>
optional<tf::Task> queue_helper(tf::Taskflow                 &task_flow,
                                const std::string            &out,
                                const cv::Mat &               depth,
                                const camera_models::pinhole &intrinsic) {
    // clang-format on
    if (!out.empty()) {
        return task_flow.emplace([&depth, &intrinsic, &out]() {
            using namespace conversion;
            // Load the image unchanged, because depth images are encoded
            // specially.
            cv::Mat bearing = depth_to_bearing<Direction>(depth, intrinsic);
            cv::imwrite(out, convert_bearing(bearing));
        });
    }
    return nullopt;
}
}  // namespace detail

int main(int argc, char **argv) {
    CLI::App app{"Convert depth images to pointclouds"};

    auto print_version = [argv](int /*count*/) {
        cout << argv[0] << " " << get_version() << "\n";
        exit(0);
    };
    app.add_flag_function("-v,--version", print_version,
                          "Print version and exit");

    string calibration_file;
    app.add_option("-c,--calibration", calibration_file,
                   "File that contains calibration parameters for the camera")
        ->required()
        ->check(CLI::ExistingFile);

    string input_file;
    app.add_option("-i,--input", input_file, "Input depth map")
        ->required()
        ->check(CLI::ExistingFile);

    string bearing_hor_name;
    app.add_option(
        "--horizontal", bearing_hor_name,
        "Calculate horizontal bearing angle image and write to this path");
    string bearing_ver_name;
    app.add_option("--vertical", bearing_ver_name,
                   "Calculate vertical bearing angle and write to this path");
    string bearing_dia_name;
    app.add_option("--diagonal", bearing_dia_name,
                   "Calculate diagonal bearing angle and write to this path");
    string bearing_ant_name;
    app.add_option(
        "--anti-diagonal", bearing_ant_name,
        "Calculate anti-diagonal bearing angle and write to this path");

    CLI11_PARSE(app, argc, argv);

    tf::Executor executor;

    optional<cv::Mat>                depth_image;
    optional<camera_models::pinhole> intrinsic;

    {
        tf::Taskflow taskflow;
        auto         start_sync = taskflow.emplace([]() {});

        auto load_img = taskflow.emplace([&input_file, &depth_image]() {
            // Load the image unchanged, because depth images are encoded
            // specially.
            depth_image = io::load_image(input_file, cv::IMREAD_UNCHANGED);
        });

        auto load_cali = taskflow.emplace([&calibration_file, &intrinsic]() {
            ifstream calibration_fstream{calibration_file};

            intrinsic = io::load_pinhole_intrinsic(calibration_fstream);
        });
        auto end_sync  = taskflow.emplace([]() {});

        start_sync.precede(load_img);
        start_sync.precede(load_cali);
        load_img.precede(end_sync);
        load_cali.precede(end_sync);

        executor.run(taskflow).wait();
    }

    if (!depth_image) {
        cerr << util::err{};
        cerr << "Could not load image \"" << rang::style::bold << input_file
             << rang::style::reset << "\"!\n";
        return 1;
    }
    if (!intrinsic) {
        cerr << util::err{};
        cerr << "Could not load intrinsic calibration \"" << rang::style::bold
             << calibration_file << rang::style::reset << "\"!\n";
        return 1;
    }

    using namespace conversion;
    const cv::Mat euclid_depth = depth_to_laserscan(*depth_image, *intrinsic);
    const camera_models::pinhole calib = *intrinsic;

    {
        tf::Taskflow taskflow;

        auto sync_start = taskflow.emplace([]() {});
        auto sync_end   = taskflow.emplace([]() {});

        using namespace ::detail;
        auto h = queue_helper<direction::horizontal>(taskflow, bearing_hor_name,
                                                     euclid_depth, calib);
        auto v = queue_helper<direction::vertical>(taskflow, bearing_ver_name,
                                                   euclid_depth, calib);
        auto d = queue_helper<direction::diagonal>(taskflow, bearing_dia_name,
                                                   euclid_depth, calib);
        auto a = queue_helper<direction::antidiagonal>(
            taskflow, bearing_ant_name, euclid_depth, calib);

        if (h) {
            sync_start.precede(*h);
            h->precede(sync_end);
        }
        if (v) {
            sync_start.precede(*v);
            v->precede(sync_end);
        }
        if (d) {
            sync_start.precede(*d);
            d->precede(sync_end);
        }
        if (a) {
            sync_start.precede(*a);
            a->precede(sync_end);
        }


        executor.run(taskflow).wait();
    }

    executor.wait_for_all();

    return 0;
}
