#include "batch_filter.h"
#include "filter_functor.h"

#include <CLI/CLI.hpp>
#include <cmath>
#include <memory>
#include <rang.hpp>
#include <sens_loc/preprocess/filter.h>
#include <sens_loc/util/console.h>
#include <sens_loc/util/correctness_util.h>
#include <sens_loc/version.h>
#include <stdexcept>
#include <util/batch_converter.h>
#include <util/colored_parse.h>
#include <util/tool_macro.h>
#include <util/version_printer.h>
#include <vector>

/// \defgroup filter-driver depth-image filtering
///
/// All code that is written to use the library and implement a program
/// that filters sensors input with various filtering techniques.

/// Parallelized driver for depth-image filtering-tool
/// \sa sens_loc::preprocess
/// \ingroup filter-driver
/// \returns 0 if all images could be converted, 1 if any image fails
MAIN_HEAD("Batch-processing tool to filter depth images and maps") {

    // Explicitly disable threading from OpenCV functions, as the
    // parallelization is done at a higher level.
    // That means, that each filter application is not multithreaded, but each
    // image modification is. This is necessary as "TaskFlow" does not play
    // nice with OpenCV threading and they introduce data races in the program
    // because of that.
    cv::setNumThreads(0);

    app.require_subcommand();  // Expect one or more filter commands
    app.footer("\n\n"
               "An example invocation of the tool is:\n"
               "\n"
               "depth_filter bilateral --input depth_{:04d}.png \\\n"
               "                       --output filtered_{:04d}.png \\\n"
               "                       --start 0 \\\n"
               "                       --end 100 \\\n"
               "                       --sigma-color 20. \\\n"
               "                       --sigma-space 20. \\\n"
               "                       --distance 5 \\\n"
               "\n"
               "This will read 'depth_0000.png ...' and filter them with the "
               "bilateral filter, producing the files 'filtered_0000.png ...'\n"
               " in the working directory");

    file_patterns files;
    app.add_option("-i,--input", files.input,
                   "Input pattern for images to filter; e.g. \"depth-{}.png\"")
        ->required();
    app.add_option(
           "-o,--output", files.output,
           "Output pattern for filtered images; e.g. \"filtered-{}.png\"")
        ->required();
    int start_idx = 0;
    app.add_option("-s,--start", start_idx, "Start index of batch, inclusive")
        ->required();
    int end_idx = 0;
    app.add_option("-e,--end", end_idx, "End index of batch, inclusive")
        ->required();

    CLI::App* bilateral_cmd = app.add_subcommand(
        "bilateral", "Apply the bilateral filter to the input.");
    bilateral_cmd->footer(
        "\n\n"
        "An example invocation of the tool is:\n"
        "\n"
        "depth_filter bilateral --input depth_{:04d}.png \\\n"
        "                       --output filtered_{:04d}.png \\\n"
        "                       --start 0 \\\n"
        "                       --end 100 \\\n"
        "                       --sigma-color 20. \\\n"
        "                       --sigma-space 20. \\\n"
        "\n"
        "This will read 'depth_0000.png ...' and filter them with the "
        "bilateral filter, producing the files 'filtered_0000.png ...'\n"
        " in the working directory");
    double sigma_color = NAN;
    bilateral_cmd->add_option("-c,--sigma-color", sigma_color,
                              "Defines threshold for color similarity.");
    int          distance        = 0;
    CLI::Option* distance_option = bilateral_cmd->add_option(
        "-d,--distance", distance,
        "One option to define the relevant neighbourhood, diameter in pixel");
    double       sigma_space = NAN;
    CLI::Option* sigma_space_option =
        bilateral_cmd->add_option("-s,--sigma-space", sigma_space,
                                  "Other option to define proximity relation, "
                                  "closeness is computed from that value");

    CLI::App* median_blur_cmd = app.add_subcommand(
        "median-blur", "Apply the median-blur filter to the input.");
    median_blur_cmd->footer(
        "\n\n"
        "An example invocation of the tool is:\n"
        "\n"
        "depth_filter median-blur --input depth_{:04d}.png \\\n"
        "                         --output filtered_{:04d}.png \\\n"
        "                         --start 0 \\\n"
        "                         --end 100 \\\n"
        "                         --distance 5 \\\n"
        "\n"
        "This will read 'depth_0000.png ...' and filter them with the "
        "median-blur filter, producing the files 'filtered_0000.png ...'\n"
        " in the working directory");
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    int kernel_size_median = 5;
    median_blur_cmd->add_set(
        "-d,--distance", kernel_size_median,
        {3, 5},  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        "The distance of pixels that are considered for the median blur.",
        /*defaulted=*/true);

    COLORED_APP_PARSE(app, argc, argv);

    if (app.got_subcommand(bilateral_cmd) && distance_option->count() == 0U &&
        sigma_space_option->count() == 0U) {
        cerr << util::err{}
             << "Provide a proximity-measure for the bilateral filter!\n";
        return 1;
    }

    // Create a vector of functors (unique_ptr<filter_interface>) that will
    // be executed in order.
    // Each element is created by one subcommand and its parameters.
    vector<unique_ptr<abstract_filter>> commands;

    for (const auto* cmd : app.get_subcommands()) {
        if (cmd->count() != 1U)
            throw std::invalid_argument{"Apply each filter only once!"};

        if (cmd == bilateral_cmd) {
            // Got at pixel-distance as argument.
            if (distance_option->count() > 0U) {
                commands.push_back(
                    make_unique<bilateral_filter>(sigma_color, distance));
            }
            // Otherwise sigma space must have been provided.
            else {
                commands.push_back(
                    make_unique<bilateral_filter>(sigma_color, sigma_space));
            }
        }

        else if (cmd == median_blur_cmd)
            commands.push_back(
                make_unique<median_blur_filter>(kernel_size_median));

        else
            UNREACHABLE("Unhandled filter provided!");  // LCOV_EXCL_LINE
    }
    Ensures(!commands.empty());

    // Batch-processing will then just execute the functors for each image.
    // It needs to take care, that the elements are moved through.
    // The final step is conversion to U16 and writing to disk.
    batch_filter process(files, commands);

    return process.process_batch(start_idx, end_idx) ? 0 : 1;
}
MAIN_TAIL
