#include <sens_loc/util/progress_bar_observer.h>

namespace sens_loc::util {
void progress_bar_observer::on_entry(unsigned /*worker_id*/,
                                     tf::TaskView /*task_view*/) {
    if (!_inital_output) {
        _inital_output = true;
        print_bar(/*increment=*/false);
    }
}

void progress_bar_observer::on_exit(unsigned /*worker_id*/,
                                    tf::TaskView /*task_view*/) {
    print_bar(/*increment=*/true);
}

void progress_bar_observer::print_bar(bool increment) noexcept {
    auto s = synced();

    if (increment)
        _done++;

    std::cout << "\r" << rang::fg::green << rang::style::bold << std::setw(5)
              << _done << " ";

    auto progress =
        gsl::narrow_cast<float>(_done) / gsl::narrow_cast<float>(_total_tasks);

    int bar_elements =
        gsl::narrow_cast<int>(progress * gsl::narrow_cast<float>(max_bars));
    Ensures(bar_elements <= max_bars);
    Ensures(bar_elements >= 0);
    int empty_elements = max_bars - bar_elements;

    std::cout << rang::style::reset << rang::bg::green << rang::fg::green;
    for (int i = 0; i < bar_elements; ++i) {
        std::cout << "#";
    }
    std::cout << rang::bg::blue << rang::fg::blue;
    for (int i = 0; i < empty_elements; ++i) {
        std::cout << ".";
    }
    std::cout << rang::style::reset << std::flush;

//    if (_done == _total_tasks)
//        std::cout << std::endl;
}
}  // namespace sens_loc::util
