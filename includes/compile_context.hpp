#pragma once

#include "type_definitions.hpp"
#include "mono_allocator.hpp"
#include <unordered_map>
#include <mutex>
#include <string_view>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

#pragma pack(push, 1)

struct compile_context {
public:
    compile_context(const string_view);
    auto add(const string_view) -> void;
    auto pop() -> pair<string_view, string_view>;
    auto cancelled() -> bool;
    auto empty() -> bool;

private:
    mutex mutex_;
    thread::id main_thread_id_;
    vector<thread> threads_;
    vector<bool> thread_states_;
    queue<string_view> file_queue_;
    mono_allocator<string, 16> module_alloc_;
    unordered_map<string_view, string*> modules_;
};

#pragma pack(pop)