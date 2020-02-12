#include "compile_context.hpp"
#include <algorithm>

using namespace std;

compile_context::compile_context(f_string<23> initial_file_path) {
    main_thread_id_ = this_thread::get_id();
    parsers_.reserve(thread::hardware_concurrency());
    file_queue_.push(initial_file_path);
}

auto compile_context::add(const f_string<23> file_path) -> void {
    mutex_.lock();
    if (modules_.find(file_path) != modules_.end()) {
        file_queue_.push(file_path);
        if (parsers_.size() < thread::hardware_concurrency()) {
            parsers_.emplace_back(this);
        }
    }
    mutex_.unlock();
}

auto compile_context::pop() -> pair<f_string<23>, string_view> {
    return { "test_file", "test_module" };
}

auto compile_context::cancel() -> void {

}

auto compile_context::cancelled() const -> bool {
    return any_of(
        parsers_.begin(),
        parsers_.end(),
        [](const auto& p) {
            return p.thr_state == thread_state::cancelled;
        }
    );
}

auto compile_context::finished() const -> bool {
    return all_of(
        parsers_.begin(),
        parsers_.end(),
        [](const auto& p) {
            return p.thr_state == thread_state::finished;
        }
    );
}

auto compile_context::empty() const -> bool {
    return file_queue_.empty();
}

auto compile_context::contains(const f_string<23> fs) const -> bool {
    return modules_.find(fs) != modules_.end();
} 