#include "compile_context.hpp"
#include <algorithm>

using namespace std;

compile_context::compile_context(const f_string<23>& initial_file_path) {
    const auto num_threads = thread::hardware_concurrency();

    parsers_.reserve(num_threads);
    threads_.reserve(num_threads - 1);
    
    auto mod = modules_.emplace(initial_file_path, module_alloc_.emplace_back(module())).first->second;
    auto& parser = parsers_.emplace_back(this);
    parser.process({ initial_file_path, mod });
}

auto compile_context::add(const f_string<23>& file_path) -> void {
    mutex_.lock();
    if (modules_.find(file_path) != modules_.end()) {
        if (threads_.size() < thread::hardware_concurrency() - 1) {
            auto mod = modules_.emplace(file_path, module_alloc_.emplace_back(module())).first->second;
            auto& parser = parsers_.emplace_back(this);
            auto& thr = threads_.emplace_back(
                std::thread([&]() { parser.process({ file_path, mod }); })
            );
            mutex_.unlock();
            thr.detach();
            return;
        } else {
            file_queue_.push(file_path);
            mutex_.unlock();
            return;
        }
    }
}

auto compile_context::pop() -> file_module {
    auto mod = modules_.emplace(
        file_queue_.front(),
        module_alloc_.emplace_back(module())
    );
    file_queue_.pop();
    return { mod.first->first, mod.first->second };
}

auto compile_context::cancel() -> void {
    mutex_.lock();
    cancelled_ = true;
    mutex_.unlock();
    
    for (auto& thr : threads_) {
        thr.join();
    }
}

auto compile_context::cancelled() const -> bool {
    return cancelled_;
}

auto compile_context::finished() const -> bool {
    return all_of(
        parsers_.begin(),
        parsers_.end(),
        [](const auto& p) {
            return p.state == parser_state::finished;
        }
    );
}

auto compile_context::empty() const -> bool {
    return file_queue_.empty();
}

auto compile_context::contains(const f_string<23>& file_path) const -> bool {
    return modules_.find(file_path) != modules_.end();
}

inline auto compile_context::lock() -> void {
    mutex_.lock();
}

inline auto compile_context::unlock() -> void {
    mutex_.unlock();
}