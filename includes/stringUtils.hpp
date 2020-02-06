#pragma once

#include <cstring>
#include <stdexcept>

namespace stringUtils {
    constexpr static auto size(const char* str) -> std::size_t {
        std::size_t l = 0;
        const char* c = str;
        while (*c != '\0') {
            ++l;
            ++c;
        }
        return l > 23 ? (throw std::logic_error("str length must be <= 23\n")) : l;
    }

    constexpr static auto hash(const char* str) -> std::size_t {
        std::size_t h = 0;
        const char* c = str;
        while (*c != '\0') {
            h <<= 7;
            h += *c;
            ++c;
        }
        return h;
    }
}