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
public:
    f_string() = default;

    f_string(const char* start, const char* end, const size_t hash) :
        hash_(hash)
    {
        const size_t length = end - start;
        if (length > N) {
            capacity_.is_small = false;
            internal_.start_ = start;
            internal_.end_ = end;
        } else {
            capacity_.is_small = true;
            capacity_.remaining = N - length;
            memcpy(&internal_.string_[0], start, length);
        }
    }

    f_string(const word& word) :
        hash_(word.hash)
    {
        const size_t length = word.end - word.start;
        if (length > N) {
            capacity_.is_small = false;
            internal_.start_ = word.start;
            internal_.end_ = word.end;
        } else {
            capacity_.is_small = true;
            capacity_.remaining = N - length;
            memcpy(&internal_.string_[0], word.start, length);
        }
    }

    f_string(string str) :
        hash_(string_utils::hash(str))
    {
        if (str.length() > N) {
            capacity_.is_small = false;
            internal_.start_ = str.data();
            internal_.end_ = &str.data()[str.length()] ;
        } else {
            capacity_.is_small = true;
            capacity_.remaining = N - str.length();
            memcpy(&internal_.string_[0], str.data(), str.length());
        }   
    }

    template<size_t S>
    constexpr f_string(const char (&str)[S]) :
        hash_(string_utils::hash(str)),
        capacity_({
            .is_small = true,
            .remaining = static_cast<int>(N - string_utils::size(str))
        })
    {
        string_utils::copy(&str[0], &str[S - 1], &internal_.string_[0]);
    }

    auto operator =(const f_string& other) -> f_string& {
        if (&other == this) {
            return *this;
        } else {
            memcpy(this, &other, sizeof(other));
            return *this;
        }
    }

    constexpr auto size() const -> u64 {
        return is_small() ? internal_string_size() : internal_.end_ - internal_.start_;
    }

    inline auto operator ==(const f_string& other) const -> bool {
        if (size() != other.size()) {
            return false;
        } else if (capacity_.is_small) {
            return memcmp(
                &internal_.string_,
                &other.internal_.string_,
                internal_string_size()) == 0;
        } else {
            return (
                (internal_.start_ == other.internal_.start_) &&
                (internal_.end_ == other.internal_.end_)
            );
        }
    }

    inline auto operator !=(const f_string& other) const -> bool {
        return !(*this == other);
    }

    friend auto operator <<(ostream& strm, const f_string& fs) -> ostream& {
        return strm << 
            (fs.is_small() 
                ? string_view(&fs.internal_.string_[0], fs.internal_string_size())
                : string_view(fs.internal_.start_, fs.internal_.end_ - fs.internal_.start_));
    }
private:
    size_t hash_;
    union {
        char string_[N];
        struct {
            char* start_;
            char* end_;
        };
    } internal_;

    constexpr static auto bits_required() -> int {
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
    } capacity_;

    constexpr auto internal_string_size() const -> u64 {
        return N - capacity_.remaining;
    }

    constexpr auto is_small() const -> bool {
        return capacity_.is_small;
    }
};

#pragma pack(pop)