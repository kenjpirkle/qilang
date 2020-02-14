#pragma once

#include "compile_context.hpp"
#include "file_module.hpp"
#include "parser_state.hpp"
#include "type_definitions.hpp"
#include "word.hpp"
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct compile_context;

#pragma pack(push, 1)

struct parser {
public:
    parser(compile_context*);
    auto process(file_module) -> void;
    parser_state state;

private:
    string source_;
    module* module_;
    vector<char*> lines_;
    compile_context* context_;
    char* peek_;

    auto watch_for_modules() -> void;
    auto read_file(f_string<23>&) -> void;
    auto lex_parse() -> void;
    auto determine_symbol() -> void;
    auto parse_id() -> word;
    auto parse_underscore_id() -> word;
    inline auto next_state() -> void;
    inline auto error(const string&) -> void;
};

#pragma pack(pop)