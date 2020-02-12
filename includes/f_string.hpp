#pragma once

#include "type_definitions.hpp"
#include "word.hpp"
#include "string_utils.hpp"
#include <string>
#include <string_view>

using namespace std;

#pragma pack(push, 1)

// f_string acts as a union of a small buffer optimized string and a string_view
template <int N = 23>
struct f_string {
    f_string() = default;

    template <int S1>
    f_string(f_string<S1>& other) :
        hash(other.hash)
    {
        u64 other_size = other.size();
        if (other_size > N) {
            capacity.is_small = false;
            internal_data.start = other.data();
            internal_data.end = &other.data()[other.is_small() ? other.internal_size() : other.external_size()];
        } else {
            capacity.is_small = true;
            capacity.remaining = N - other_size;
            memcpy(
                &internal_data.string[0],
                other.data(),
                other_size
            );
        }
    }

    f_string(const char* start, const char* end, const size_t hash) :
        hash(hash)
    {
        const size_t length = end - start;
        if (length > N) {
            capacity.is_small = false;
            internal_data.start_ = start;
            internal_data.end_ = end;
        } else {
            capacity.is_small = true;
            capacity.remaining = N - length;
            memcpy(&internal_data.string[0], start, length);
        }
    }

    f_string(const word& word) :
        hash(word.hash)
    {
        const size_t length = word.end - word.start;
        if (length > N) {
            capacity.is_small = false;
            internal_data.start = word.start;
            internal_data.end = word.end;
        } else {
            capacity.is_small = true;
            capacity.remaining = N - length;
            memcpy(&internal_data.string[0], word.start, length);
        }
    }

    f_string(string str) :
        hash(string_utils::hash(str))
    {
        if (str.length() > N) {
            capacity.is_small = false;
            internal_data.start = str.data();
            internal_data.end = &str.data()[str.length()] ;
        } else {
            capacity.is_small = true;
            capacity.remaining = N - str.length();
            memcpy(&internal_data.string[0], str.data(), str.length());
        }   
    }

    template<size_t S>
    constexpr f_string(const char (&str)[S]) :
        hash(string_utils::hash(str)),
        capacity({
            .is_small = true,
            .remaining = static_cast<int>(N - string_utils::size(str))
        })
    {
        string_utils::copy(&str[0], &str[S - 1], &internal_data.string[0]);
    }

    auto operator =(const f_string& other) -> f_string& {
        if (&other == this) {
            return *this;
        } else {
            memcpy(this, &other, sizeof(other));
            return *this;
        }
    }
    
    template <int S1>
    inline auto operator ==(const f_string<S1>& other) const -> bool {
        if (is_small()) {
            if (other.is_small()) {
                if (internal_size() != other.internal_size()) {
                    return false;
                } else {
                    return memcmp(
                        &internal_data.string[0],
                        &other.internal_data.string[0],
                        internal_size()
                    ) == 0;
                }
            } else {
                if (internal_size() != other.external_size()) {
                    return false;
                } else {
                    return memcmp(
                        &internal_data.string[0],
                        other.internal_data.start,
                        internal_size()
                    ) == 0;
                }
            }
        } else {
            if (other.is_small()) {
                if (external_size() != other.internal_size()) {
                    return false;
                } else {
                    return memcmp(
                        internal_data.start,
                        &other.internal_data.string[0],
                        other.internal_size()
                    ) == 0;
                }
            } else {
                if (external_size() != other.external_size()) {
                    return false;
                } else {
                    return memcmp(
                        internal_data.start,
                        &other.internal_data.start,
                        external_size()
                    ) == 0;
                }
            }
        }
    }

    template <int S1>
    inline auto operator !=(const f_string<S1>& other) const -> bool {
        return !(*this == other);
    }

    inline auto operator ==(const f_string& other) const -> bool {
        if (size() != other.size()) {
            return false;
        } else if (is_small()) {
            return memcmp(
                &internal_data.string,
                &other.internal_data.string,
                internal_size()) == 0;
        } else {
            return internal_data.start == other.internal_data.start;
        }
    }

    inline auto operator !=(const f_string& other) const -> bool {
        return !(*this == other);
    }

    friend inline auto operator <<(ostream& strm, const f_string& fs) -> ostream& {
        return strm << 
            (fs.is_small() 
                ? string_view(&fs.internal_data.string[0], fs.internal_size())
                : string_view(fs.internal_data.start, fs.internal_data.end - fs.internal_data.start));
    }

    size_t hash;
    union {
        char string[N];
        struct {
            char* start;
            char* end;
        };
    } internal_data;

    constexpr inline static auto bits_required() -> int {
        int value = N;
        int count = 0;
        while (value > 0) {
            count++;
            value = value >> 1;
        }
        return count + 1;
    }

    struct small_capacity {
        bool is_small : 1;
        int remaining : bits_required();
    } capacity;

    constexpr inline auto internal_size() const -> u64 {
        return N - capacity.remaining;
    }

    constexpr inline auto external_size() const -> u64 {
        return internal_data.end - internal_data.start;
    }

    constexpr inline auto is_small() const -> bool {
        return capacity.is_small;
    }

    constexpr inline auto size() const -> u64 {
        return is_small() ? internal_size() : external_size();
    }

    constexpr inline auto data() -> char* {
        return is_small() ? &internal_data.string[0] : internal_data.start;
    }
};

#pragma pack(pop)

template <int N>
struct f_string_hasher {
    inline auto operator ()(const f_string<N>& fs) const -> std::size_t {
        return fs.hash;
    }
};