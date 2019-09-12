#include <CLI/CLI.hpp>
#include <chrono>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <opencv2/imgcodecs.hpp>
#include <rang.hpp>
#include <sens_loc/conversion/depth_to_bearing.h>
#include <sens_loc/conversion/depth_to_laserscan.h>
#include <sens_loc/io/image.h>
#include <sens_loc/io/intrinsics.h>
#include <sens_loc/util/console.h>
#include <sens_loc/version.h>
#include <string>
#include <taskflow/taskflow.hpp>
#include <vector>

using namespace sens_loc;
using namespace std;

namespace {
struct file_patterns {
    string input;
    string output;
    string horizontal;
    string vertical;
    string diagonal;
    string antidiagonal;
};
}  // namespace

namespace bearing {
/// This file substitues 'index' into the patterns (if existent) and calculates
/// bearing angle images for it.
/// If any operation fails 'false' is returned. On success 'true' is returned.
bool process_file(const file_patterns &         p,
                  const camera_models::pinhole &intrinsic, int index) {
    Expects(!p.input.empty());
    Expects(!p.horizontal.empty() || !p.vertical.empty() ||
            !p.diagonal.empty() || !p.antidiagonal.empty());

    using namespace conversion;

    const std::string input_file = fmt::format(p.input, index);
    optional<cv::Mat> depth_image =
        io::load_image(input_file, cv::IMREAD_UNCHANGED);

    if (!depth_image)
        return false;

    const cv::Mat euclid_depth =
        depth_to_laserscan<double, ushort>(*depth_image, intrinsic);

    bool final_result = true;
#define BEARING_PROCESS(DIRECTION)                                             \
    if (!p.DIRECTION.empty()) {                                                \
        cv::Mat bearing =                                                      \
            depth_to_bearing<direction::DIRECTION, double, double>(            \
                euclid_depth, intrinsic);                                      \
        bool success = cv::imwrite(fmt::format(p.DIRECTION, index),            \
                                   convert_bearing<double, ushort>(bearing));  \
        final_result &= success;                                               \
    }

    BEARING_PROCESS(horizontal)
    BEARING_PROCESS(vertical)
    BEARING_PROCESS(diagonal)
    BEARING_PROCESS(antidiagonal)

#undef BEARING_PROCESS

    return final_result;
}

/// Executor for the batch conversion of depth images to bearing angle images.
int convert_bearing_batch(const file_patterns &files, int start_idx,
                          int                           end_idx,
                          const camera_models::pinhole &intrinsic) {
    if (files.horizontal.empty() && files.vertical.empty() &&
        files.diagonal.empty() && files.antidiagonal.empty()) {
        cerr << util::err{};
        cerr << "At least one output pattern required!\n";
        return 1;
    }

    int fails       = 0;
    int return_code = 0;
    {
        tf::Executor executor;
        tf::Taskflow tf;
        mutex        cout_mutex;

        tf.parallel_for(
            start_idx, end_idx + 1, 1,
            [&files, &cout_mutex, &intrinsic, &return_code, &fails](int idx) {
                const bool success = process_file(files, intrinsic, idx);
                if (!success) {
                    lock_guard l(cout_mutex);
                    fails++;
                    cerr << util::err{};
                    cerr << "Could not process index \"" << rang::style::bold
                         << idx << "\"" << rang::style::reset << "!\n";
                    return_code = 1;
                }
            });

        const auto before = chrono::steady_clock::now();
        executor.run(tf).wait();
        const auto after = chrono::steady_clock::now();

        cerr << util::info{};
        cerr << "Processing " << rang::style::bold
             << end_idx - start_idx + 1 - fails << rang::style::reset
             << " images took " << rang::style::bold
             << chrono::duration_cast<chrono::seconds>(after - before).count()
             << "" << rang::style::reset << " seconds!\n";

        if (fails > 0)
            cerr << util::warn{} << "Encountered " << rang::style::bold << fails
                 << rang::style::reset << " problematic files!\n";
    }

    return return_code;
}
}  // namespace bearing

namespace range {
bool process_file(const file_patterns &         p,
                  const camera_models::pinhole &intrinsic, int index) {
    Expects(!p.input.empty());
    Expects(!p.output.empty());

    using namespace conversion;

    const std::string input_file = fmt::format(p.input, index);
    optional<cv::Mat> depth_image =
        io::load_image(input_file, cv::IMREAD_UNCHANGED);

    if (!depth_image)
        return false;

    const cv::Mat euclid_depth =
        depth_to_laserscan<double, ushort>(*depth_image, intrinsic);

    cv::Mat depth_converted((*depth_image).rows, (*depth_image).cols,
                            (*depth_image).type());
    euclid_depth.convertTo(depth_converted, (*depth_image).type());

    bool success = cv::imwrite(fmt::format(p.output, index), depth_converted);

    return success;
}

/// Executor for the batch conversion of depth images to bearing angle images.
int convert_bearing_batch(const file_patterns &files, int start_idx,
                          int                           end_idx,
                          const camera_models::pinhole &intrinsic) {
    if (files.output.empty()) {
        cerr << util::err{};
        cerr << "output pattern required!\n";
        return 1;
    }

    int fails       = 0;
    int return_code = 0;
    {
        tf::Executor executor;
        tf::Taskflow tf;
        mutex        cout_mutex;

        tf.parallel_for(
            start_idx, end_idx + 1, 1,
            [&files, &cout_mutex, &intrinsic, &return_code, &fails](int idx) {
                const bool success = process_file(files, intrinsic, idx);
                if (!success) {
                    lock_guard l(cout_mutex);
                    fails++;
                    cerr << util::err{};
                    cerr << "Could not process index \"" << rang::style::bold
                         << idx << "\"" << rang::style::reset << "!\n";
                    return_code = 1;
                }
            });

        const auto before = chrono::steady_clock::now();
        executor.run(tf).wait();
        const auto after = chrono::steady_clock::now();

        cerr << util::info{};
        cerr << "Processing " << rang::style::bold
             << end_idx - start_idx + 1 - fails << rang::style::reset
             << " images took " << rang::style::bold
             << chrono::duration_cast<chrono::seconds>(after - before).count()
             << "" << rang::style::reset << " seconds!\n";

        if (fails > 0)
            cerr << util::warn{} << "Encountered " << rang::style::bold << fails
                 << rang::style::reset << " problematic files!\n";
    }

    return return_code;
}

}  // namespace range

int main(int argc, char **argv) {
    CLI::App app{"Batchconversion of depth images to bearing angle images."};

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
    file_patterns files;

    app.add_option("-i,--input", files.input,
                   "Input pattern for image, e.g. \"depth-{}.png\"")
        ->required();

    string input_type = "pinhole-depth";
    app.add_set("-t,--type", input_type, {"pinhole-depth", "pinhole-range"});

    int start_idx;
    app.add_option("-s,--start", start_idx, "Start index of batch, inclusive")
        ->required();
    int end_idx;
    app.add_option("-e,--end", end_idx, "End index of batch, inclusive")
        ->required();

    // Bearing angle images territory
    CLI::App *bearing_cmd = app.add_subcommand(
        "bearing", "Converts depth images into bearing angle images");
    bearing_cmd->add_option(
        "--horizontal", files.horizontal,
        "Calculate horizontal bearing angle image and write to this pattern");
    bearing_cmd->add_option(
        "--vertical", files.vertical,
        "Calculate vertical bearing angle and write to this pattern");
    bearing_cmd->add_option(
        "--diagonal", files.diagonal,
        "Calculate diagonal bearing angle and write to this pattern");
    bearing_cmd->add_option(
        "--anti-diagonal", files.antidiagonal,
        "Calculate anti-diagonal bearing angle and write to this pattern");

    // Range images territory
    CLI::App *range_cmd = app.add_subcommand(
        "range", "Convert depth images into range images (laser-scan like)");
    range_cmd->add_option("-o,--output", files.output,
                          "Output pattern for the range images.");

    CLI11_PARSE(app, argc, argv);

    if (files.input.empty()) {
        cerr << util::err{}
             << "Input pattern for files to process is required!\n";
        return 1;
    }

    // Options that are always required are checked first.
    ifstream                         calibration_fstream{calibration_file};
    optional<camera_models::pinhole> intrinsic =
        io::load_pinhole_intrinsic(calibration_fstream);

    if (!intrinsic) {
        cerr << util::err{};
        cerr << "Could not load intrinsic calibration \"" << rang::style::bold
             << calibration_file << rang::style::reset << "\"!\n";
        return 1;
    }

    if (*bearing_cmd)
        return bearing::convert_bearing_batch(files, start_idx, end_idx,
                                              *intrinsic);
    else if (*range_cmd)
        return range::convert_bearing_batch(files, start_idx, end_idx,
                                            *intrinsic);
    else {
        cerr << util::err{};
        cerr << "One subcommand is required!\n";
        return 1;
    }
}
