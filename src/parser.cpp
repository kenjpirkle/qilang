#include "parser.hpp"

parser::parser(compile_context* context, f_string<23> file_path, string* mod) :
    context_(context)
{
    process(file_path, mod);
    watch_for_modules();
}

auto parser::process(f_string<23> file_path, string* mod) -> void {

}

auto parser::watch_for_modules() -> void {
    pair<f_string<23>, string_view> file_module;
    do {
        if (context_->try_lock()) {
            if (!context_->empty()) {
                file_module = context_->pop();
                state = parser_state::working;
                context_->unlock();
                // parse file

            } else {
                state = parser_state::finished;
                if (context_->cancelled() || context_->finished()) {
                    context_->unlock();
                    return;
                }
                context_->unlock();
            }
        }
    } while (true);
}