#include "compile_context.hpp"

using namespace std;

compile_context::compile_context(const string_view initial_file_path) {
    main_thread_id_ = this_thread::get_id();
    threads_.reserve(thread::hardware_concurrency());
    file_queue_.push(initial_file_path);
}

auto compile_context::add(const string_view file_path) -> void {
    mutex_.lock();
    if (modules_.find(file_path) != modules_.end()) {
        file_queue_.push(file_path);
        if (threads_.size() < thread::hardware_concurrency()) {
            threads_.emplace_back(thread(
                [&]() {
                    // start a parser here
                }
            )).detach();
        }
    }
    mutex_.unlock();
}

auto compile_context::pop() -> pair<string_view, string_view> {
    return { "test_file", "test_module" };
}

auto compile_context::cancelled() -> bool {
    return false;
}