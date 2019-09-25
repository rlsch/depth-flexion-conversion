#define NONIUS_RUNNER 1
#include "util.h"

#include <nonius/nonius_single.h++>
#include <sens_loc/conversion/depth_to_laserscan.h>
#include <sens_loc/io/image.h>

using namespace sens_loc;
using namespace conversion;

NONIUS_BENCHMARK("Depth2Euclidean", [](nonius::chronometer meter) {
    const auto [depth, _, p] = get_data();
    (void) _;
    cv::Mat                          in   = depth;
    sens_loc::camera_models::pinhole cali = p;
    meter.measure([&] { return depth_to_laserscan(in, cali); });
})

NONIUS_BENCHMARK("Depth2Euclidean parallel", [](nonius::chronometer meter) {
    const auto [depth, euclid, p]         = get_data();
    cv::Mat                          in   = depth;
    cv::Mat                          out  = euclid;
    sens_loc::camera_models::pinhole cali = p;
    tf::Executor                     exe;
    tf::Taskflow                     flow;
    meter.measure([&] {
        par_depth_to_laserscan(in, cali, out, flow);
        exe.run(flow).wait();
        flow.clear();
    });
})
