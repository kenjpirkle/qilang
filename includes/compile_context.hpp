#pragma once

#include "f_string.hpp"
#include "mono_allocator.hpp"
#include "parser.hpp"
#include "thread_state.hpp"
#include "type_definitions.hpp"
#include <unordered_map>
#include <mutex>
#include <string_view>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

struct parser;

#pragma pack(push, 1)

struct compile_context {
public:
    compile_context(const f_string<23>);
    auto add(const f_string<23>) -> void;
    auto pop() -> pair<f_string<23>, string_view>;
    auto cancel() -> void;
    auto cancelled() const -> bool;
    auto finished() const -> bool;
    auto empty() const -> bool;
    auto contains(const f_string<23>) const -> bool;

private:
    mutex mutex_;
    thread::id main_thread_id_;
    vector<parser> parsers_;
    queue<f_string<23>> file_queue_;
    mono_allocator<f_string<23>, 16> module_alloc_;
    unordered_map<f_string<23>, string*, f_string_hasher<23>> modules_;
};

#pragma pack(pop)