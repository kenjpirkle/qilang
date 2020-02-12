#pragma once

#include "compile_context.hpp"
#include "f_string.hpp"
#include "thread_state.hpp"
#include "type_definitions.hpp"
#include "word.hpp"
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

using namespace std;

struct compile_context;

#pragma pack(push, 1)

struct parser {
public:
    parser(compile_context*);
    thread_state thr_state;

private:
    thread thread_;
    string source_;
    vector<char*> lines_;
    compile_context* context_;
    char* peek_;

    auto watch_for_modules() -> void;
    auto process(const f_string<>) -> void;
    auto read_file(const f_string<>) -> void;
    auto lex_parse() -> void;
    auto determine_symbol() -> void;
    auto parse_id() -> word;
    auto parse_underscore_id() -> word;
    inline auto next_state() -> void;
    inline auto error(const string&) -> void;
};

#pragma pack(pop)