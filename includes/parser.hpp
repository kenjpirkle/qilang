#pragma once

#include "builtin_type.hpp"
#include "compile_context.hpp"
#include "file_module.hpp"
#include "keyword.hpp"
#include "type_definitions.hpp"
#include "word.hpp"
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std;

struct compile_context;

static const unordered_map<f_string<23>, keyword, f_string_hasher<23>> keywords = {
    { "if",     keyword::if_kw     },
    { "else",   keyword::else_kw   },
    { "for",    keyword::for_kw    },
    { "return", keyword::return_kw },
    { "type",   keyword::type_kw   },
    { "enum",   keyword::enum_kw   },
    { "union",  keyword::union_kw  },
    { "defer",  keyword::defer_kw  }
};

static const unordered_map<f_string<23>, builtin_type, f_string_hasher<23>> builtin_types = {
    { "int",    builtin_type::int_t    },
    { "i8",     builtin_type::i8_t     },
    { "i16",    builtin_type::i16_t    },
    { "i32",    builtin_type::i32_t    },
    { "i64",    builtin_type::i64_t    },
    { "i128",   builtin_type::i128_t   },
    { "uint",   builtin_type::uint_t   },
    { "u8",     builtin_type::u8_t     },
    { "u16",    builtin_type::u16_t    },
    { "u32",    builtin_type::u32_t    },
    { "u64",    builtin_type::u64_t    },
    { "u128",   builtin_type::u128_t   },
    { "float",  builtin_type::float_t  },
    { "f32",    builtin_type::f32_t    },
    { "f64",    builtin_type::f64_t    },
    { "f128",   builtin_type::f128_t   },
    { "bool",   builtin_type::bool_t   },
    { "char",   builtin_type::char_t   },
    { "string", builtin_type::string_t }
};

#pragma pack(push, 1)

struct parser {
public:
    parser(compile_context*);
    auto process(file_module) -> void;
    inline auto finished() const -> bool;

private:
    bool finished_;
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