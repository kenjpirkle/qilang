#pragma once

#include "type_definitions.hpp"
#include "word.hpp"
#include "string_utils.hpp"
#include <string_view>

using namespace std;

#pragma pack(push, 1)

// f_string acts as a union of a small buffer optimized string and a string_view
struct f_string {
public:
    f_string(const char* start, const char* end, const size_t hash) :
        hash_(hash)
    {
        const size_t length = end - start;
        if (length > 23) {
            capacity_.is_small = false;
            string_view_ = string_view(start, length);
        } else {
            capacity_.is_small = true;
            capacity_.remaining = 23 - length;
            memcpy(&internal_string_[0], start, length);
        }
    }

    f_string(const word word) :
        hash_(word.hash)
    {
        const size_t length = word.end - word.start;
        if (length > 23) {
            capacity_.is_small = false;
            string_view_ = string_view(word.start, length);
        } else {
            capacity_.is_small = true;
            capacity_.remaining = 23 - length;
            memcpy(&internal_string_[0], word.start, length);
        }
    }

    template<size_t N>
    constexpr f_string(const char (&str)[N]) :
        hash_(string_utils::hash(str)),
        internal_string_(),
        capacity_({
            .is_small = true,
            .remaining = static_cast<char>(23 - (string_utils::size(str)))
        })
    {
        string_utils::copy(&str[0], &str[N - 1], &internal_string_[0]);
    }

    constexpr auto size() const -> u64 {
        return is_small() ? internal_string_size() : string_view_.length();
    }

    inline auto operator ==(const f_string& other) const -> bool {
        if (size() != other.size()) {
            return false;
        } else if (capacity_.is_small) {
            return memcmp(
                &internal_string_,
                &other.internal_string_,
                internal_string_size()) == 0;
        } else {
            return memcmp(
                &string_view_[0],
                &other.string_view_[0],
                string_view_.size()) == 0;
        }
    }

    inline auto operator !=(const f_string& other) const -> bool {
        return !(*this == other);
    }

    friend auto operator <<(ostream& strm, const f_string& fs) -> ostream& {
        return strm << 
            (fs.is_small() 
                ? string_view(&fs.internal_string_[0], fs.internal_string_size())
                : fs.string_view_);
    }
private:
    const size_t hash_;
    union {
        char internal_string_[23];
        string_view string_view_;
    };
    struct small_capacity {
        bool is_small   : 1;
        char remaining : 7;
    } capacity_;

    constexpr auto internal_string_size() const -> u64 {
        return 23 - capacity_.remaining;
    }

    constexpr auto is_small() const -> bool {
        return capacity_.is_small;
    }
};

#pragma pack(pop)