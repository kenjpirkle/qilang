#include "parser.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

parser::parser(compile_context* context, file_module) :
    context_(context)
{
    watch_for_modules();
}

auto parser::watch_for_modules() -> void {
    file_module file_mod;
    do {
        if (context_->try_lock()) {
            if (!context_->empty()) {
                file_mod = context_->pop();
                state = parser_state::working;
                context_->unlock();
                read_file(file_mod.file_path);
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

auto parser::read_file(f_string<23> file_path) -> void {
    // TODO: f_string needs null terminator to avoid converting to string_view
    string_view sv(file_path.data(), file_path.size());
    ifstream in(sv.data(), ios::in | ios::binary);
    if (in) {
        in.seekg(0, ios::end);
        source_.resize(1 + (int)in.tellg());
        in.seekg(0, ios::beg);
        in.read(&source_[0], source_.size());
        source_.back() = '\0';
        in.close();
    } else {
        throw(errno);
    }
}