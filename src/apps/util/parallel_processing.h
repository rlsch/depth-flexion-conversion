#ifndef PARALLEL_PROCESSING_H_2FVRLMCH
#define PARALLEL_PROCESSING_H_2FVRLMCH

#include <chrono>
#include <gsl/gsl>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sens_loc/util/console.h>
#include <sens_loc/util/progress_bar_observer.h>
#include <taskflow/taskflow.hpp>
#include <type_traits>

namespace sens_loc::apps {

/// Helper function that processes a range of files based on index.
/// The boolean function \c f is applied to each function. Error handling
/// and reporting is done if \c f returns \c false.
///
/// \tparam BoolFunction Apply this functor for each index.
/// \param start,end inclusive range of integers for the files
/// \param f functor that is applied for each index
template <typename BoolFunction>
bool parallel_indexed_file_processing(int          start,
                                      int          end,
                                      BoolFunction f) noexcept {
    static_assert(std::is_nothrow_invocable_r_v<bool, BoolFunction, int>,
                  "Functor needs to be noexcept callable and return bool!");

    try {
        if (start > end)
            std::swap(start, end);

        int total_tasks = end - start + 1;

        tf::Executor executor;
        executor.make_observer<util::progress_bar_observer>(total_tasks);
        tf::Taskflow tf;

        bool batch_success = true;
        int  fails         = 0;

        tf.parallel_for(
            start, end + 1, 1, [&batch_success, &fails, &f](int idx) {
                const bool success = f(idx);
                if (!success) {
                    auto s = synced();
                    fails++;
                    std::cerr << util::err{};
                    std::cerr << "Could not process index \""
                              << rang::style::bold << idx << "\""
                              << rang::style::reset << "!" << std::endl;
                    batch_success = false;
                }
            });

        const auto before = std::chrono::steady_clock::now();
        executor.run(tf).wait();
        const auto after = std::chrono::steady_clock::now();
        const auto dur_deci_seconds =
            std::chrono::duration_cast<std::chrono::duration<long, std::centi>>(
                after - before);
        std::cout << std::endl;

        Ensures(fails >= 0);

        using namespace std::chrono;
        {
            auto s = synced();
            std::cerr << util::info{};
            std::cerr << "Processing " << rang::style::bold
                      << std::abs(end - start) + 1 - fails << rang::style::reset
                      << " images took " << rang::style::bold << std::fixed
                      << std::setprecision(2)
                      << (dur_deci_seconds.count() / 100.) << rang::style::reset
                      << " seconds!\n\n";
        }

        if (fails > 0) {
            auto s = synced();
            std::cerr << util::warn{} << "Encountered " << rang::style::bold
                      << fails << rang::style::reset << " problematic files!\n";
        }

        return batch_success;
    } catch (...) {
        auto s = synced();
        std::cerr << util::err{} << "System error in batch processing!\n";
        return false;
    }
}

}  // namespace sens_loc::apps

#endif /* end of include guard: PARALLEL_PROCESSING_H_2FVRLMCH */
