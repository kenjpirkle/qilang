#include "compileContext.hpp"

compileContext::compileContext(const std::string_view initialFilePath) {
    mainThreadId_ = std::this_thread::get_id();
    threads_.reserve(std::thread::hardware_concurrency());
    fileQueue_.push(initialFilePath);
}

auto compileContext::add(const std::string_view filePath) -> void {
    mutex_.lock();
    if (modules_.find(filePath) != modules_.end()) {
        fileQueue_.push(filePath);
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