#include "compile_context.hpp"

compile_context::compile_context(const std::string_view initial_file_path) {
    main_thread_id_ = std::this_thread::get_id();
    threads_.reserve(std::thread::hardware_concurrency());
    file_queue_.push(initial_file_path);
}

auto compile_context::add(const std::string_view file_path) -> void {
    mutex_.lock();
    if (modules_.find(file_path) != modules_.end()) {
        file_queue_.push(file_path);
        if (threads_.size() < std::thread::hardware_concurrency()) {
            threads_.emplace_back(std::thread(
                [&]() {
                    // start a parser here
                }
            )).detach();
        }
    }
    mutex_.unlock();
}