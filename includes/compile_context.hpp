#pragma once

#include "file_module.hpp"
#include "mono_allocator.hpp"
#include "parser.hpp"
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
    compile_context(const f_string<23>&);
    auto add(const f_string<23>&) -> void;
    auto pop() -> file_module;
    auto cancel() -> void;
    auto cancelled() const -> bool;
    auto finished() const -> bool;
    auto empty() const -> bool;
    auto contains(const f_string<23>&) const -> bool;
    auto lock() -> void;
    auto unlock() -> void;

private:
    mutex mutex_;
    vector<thread> threads_;
    vector<parser> parsers_;
    bool cancelled_;
    queue<f_string<23>> file_queue_;
    mono_allocator<module, 16> module_alloc_;
    unordered_map<f_string<23>, module*, f_string_hasher<23>> modules_;
};

#pragma pack(pop)