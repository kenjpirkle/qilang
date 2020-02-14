#include "compile_context.hpp"
#include <algorithm>

using namespace std;

compile_context::compile_context(const f_string<23> initial_file_path) {
    const auto num_threads = thread::hardware_concurrency();

    parsers_.reserve(num_threads);
    threads_.reserve(num_threads - 1);
    
    auto mod = modules_.emplace(initial_file_path, new string).first->second;
    parsers_.emplace_back(this, initial_file_path, mod);
}

auto compile_context::add(const f_string<23> file_path) -> void {
    do {
        if (try_lock()) {
            if (modules_.find(file_path) != modules_.end()) {
                if (threads_.size() < thread::hardware_concurrency() - 1) {
                    auto mod = modules_.emplace(file_path, new string()).first->second;
                    auto& thr = threads_.emplace_back(
                        std::thread(
                            [&]() {
                                parsers_.emplace_back(this, file_path, mod);
                            }
                        )
                    );
                    unlock();
                    thr.detach();
                    return;
                } else {
                    file_queue_.push(file_path);
                    unlock();
                    return;
                }
            }
        }
    } while (true);
}

auto compile_context::pop() -> pair<f_string<23>, string_view> {
    return { "test_file", "test_module" };
}

auto compile_context::cancel() -> void {
    for (auto& thr : threads_) {
        thr.join();
    }
}

auto compile_context::cancelled() const -> bool {
    return any_of(
        parsers_.begin(),
        parsers_.end(),
        [](const auto& p) {
            return p.state == parser_state::cancelled;
        }
    );
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

auto compile_context::contains(const f_string<23> fs) const -> bool {
    return modules_.find(fs) != modules_.end();
}

auto compile_context::try_lock() -> bool {
    return mutex_.try_lock();
}

auto compile_context::unlock() -> void {
    mutex_.unlock();
}