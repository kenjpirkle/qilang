#include "parser.hpp"

parser::parser(compile_context* context) :
    context_(context)
{
    thread_ = std::thread([&]() {
        watch_for_modules();
    });
    thread_.detach();
}

auto parser::watch_for_modules() -> void {
    pair<f_string<23>, string_view> file_module;
    do {
        if (!context_->empty()) {
            file_module = context_->pop();
            // parse file
        }
    } while (!context_->cancelled());
}