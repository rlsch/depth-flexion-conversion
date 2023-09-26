#ifndef BATCH_VISITOR_H_WIWKQDOS
#define BATCH_VISITOR_H_WIWKQDOS

#include <chrono>
#include <iomanip>
#include <sens_loc/util/progress_bar_observer.h>
#include <system_error>
#include <taskflow/taskflow.hpp>
#include <type_traits>

namespace sens_loc::apps {

/// General purpose function to execute a specific visitor in parallel
/// in order to determine some statistical insight.
/// \tparam Functor Apply this functor for each index.
/// \param start,end inclusive range of integers for the files to be accessed.
/// \param f functor that is applied for each index
template <typename Functor>
Functor parallel_visitation(int start, int end, Functor&& f) noexcept {
    static_assert(std::is_nothrow_invocable_r_v<void, Functor, int>,
                  "Functor needs to be noexcept callable with an 'int' and "
                  "return nothing!");

    if (start > end)
        std::swap(start, end);

    tf::Executor executor;

    int total_tasks = end - start + 1;
    executor.make_observer<util::progress_bar_observer>(total_tasks);
    tf::Taskflow tf;
    tf.parallel_for(start, end + 1, 1, std::forward<Functor>(f));
    const auto before = std::chrono::steady_clock::now();
    executor.run(tf).wait();
    const auto after = std::chrono::steady_clock::now();
    const auto dur_deci_seconds =
        std::chrono::duration_cast<std::chrono::duration<long, std::centi>>(
            after - before);

    std::cout << std::endl;
    {
        auto s = synced();
        std::cerr << util::info{};
        std::cerr << "Processing " << rang::style::bold
                  << std::abs(end - start) + 1 << rang::style::reset
                  << " images took " << rang::style::bold << std::fixed
                  << std::setprecision(2) << (dur_deci_seconds.count() / 100.)
                  << rang::style::reset << " seconds!\n\n";
    }

    return std::forward<Functor>(f);
}

}  // namespace sens_loc::apps

#endif /* end of include guard: BATCH_VISITOR_H_WIWKQDOS */
