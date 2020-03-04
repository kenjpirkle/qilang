#include "parser.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

// TODO: parser needs a mutex to prevent data races on the finished call

parser::parser(compile_context* context) : context_(context) {}

auto parser::process(file_module file_mod) -> void {
    read_file(file_mod.file_path);
    lex_parse();
    finished_ = true;
    watch_for_modules();
}

auto parser::finished() const -> bool {
    return finished_;
}

auto parser::watch_for_modules() -> void {
    file_module file_mod;

    do {
        context_->lock();
        if (!context_->cancelled()) {
            if (!context_->empty()) {
                file_mod = context_->pop();
                finished_ = false;
                context_->unlock();
                module_ = file_mod.module_ref;
                read_file(file_mod.file_path);
                finished_ = true;
            } else if (context_->finished()) {
                context_->unlock();
                break;
            } else {
                context_->unlock();
            }
        } else {
            context_->unlock();
            break;
        }
    } while (true);
}

auto parser::read_file(f_string<23>& file_path) -> void {
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
        cout << "file: " << file_path << " could not be opened or does not exist\n";
        context_->cancel();
    }
}

auto parser::lex_parse() -> void {

}