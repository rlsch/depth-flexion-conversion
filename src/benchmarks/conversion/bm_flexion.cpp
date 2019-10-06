#define NONIUS_RUNNER 1
#include "util.h"

#include <nonius/nonius_single.h++>
#include <sens_loc/conversion/depth_to_flexion.h>

using namespace sens_loc;
using namespace conversion;


NONIUS_BENCHMARK("Depth2Flexion", [](nonius::chronometer meter) {
    const auto [_, euclid, p] = get_data();
    (void) _;
    cv::Mat                          in   = euclid;
    sens_loc::camera_models::pinhole cali = p;
    meter.measure([&] { return depth_to_flexion(in, cali); });
})

NONIUS_BENCHMARK("Depth2Flexion Parallel", [](nonius::chronometer meter) {
    const auto [_, euclid, p] = get_data();
    (void) _;

    cv::Mat                          in   = euclid;
    cv::Mat                          out  = euclid;
    sens_loc::camera_models::pinhole cali = p;
    tf::Executor                     exe;
    tf::Taskflow                     flow;

    meter.measure([&] {
        par_depth_to_flexion(in, cali, out, flow);
        exe.run(flow).wait();
        flow.clear();
    });
})
