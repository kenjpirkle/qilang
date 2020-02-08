#pragma once

#include "type_definitions.hpp"
#include "mono_allocator.hpp"
#include <optional>
#include <unordered_map>
#include <mutex>
#include <string_view>
#include <queue>
#include <thread>
#include <vector>

#pragma pack(push, 1)

struct compile_context {
public:
    compile_context(const std::string_view);
    auto add(const std::string_view) -> void;
    auto pop() -> std::optional<std::pair<std::string_view, std::string_view>>;

private:
    std::mutex mutex_;
    std::thread::id main_thread_id_;
    std::vector<std::thread> threads_;
    std::vector<bool> thread_states_;
    std::queue<std::string_view> file_queue_;
    mono_allocator<std::string, 8> module_alloc_;
    std::unordered_map<std::string_view, std::string*> modules_;
};

#pragma pack(pop)