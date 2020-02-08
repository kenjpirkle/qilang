#pragma once

#include <cstring>
#include <stdexcept>

using namespace std;

namespace string_utils {
    constexpr static auto size(const char* str) -> size_t {
        size_t l = 0;
        const char* c = str;
        while (*c != '\0') {
            ++l;
            ++c;
        }
        return l > 23 ? (throw logic_error("str length must be <= 23\n")) : l;
    }

    constexpr static auto hash(const char* str) -> size_t {
        size_t h = 0;
        const char* c = str;
        while (*c != '\0') {
            h <<= 7;
            h += *c;
            ++c;
        }
        return h;
    }

    constexpr static auto copy(const char* first, const char* last, char* to) -> void {
        while (first < last) {
            *to++ = *first++;
        }
    }
}