#include "parser.hpp"

parser::parser(compile_context* context) :
    context_(context)
{
    watch_for_modules();
}

auto parser::watch_for_modules() -> void {
    optional<pair<string_view, string_view>> file_module;
    do {
        file_module = context_->pop();
    } while (!context_->cancelled());
}