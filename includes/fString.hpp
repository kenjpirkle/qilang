#pragma once

#include "typeDefinitions.hpp"
#include "word.hpp"
#include "stringUtils.hpp"
#include <string_view>

#pragma pack(push, 1)

// fString acts as a union of a small buffer optimized string and a string_view
struct fString {
public:
    fString(const char* start, const char* end, const std::size_t hash) :
        hash_(hash)
    {
        const std::size_t length = end - start;
        if (length > 23) {
            capacity_.isSmall = false;
            stringView_ = std::string_view(start, length);
        } else {
            capacity_.isSmall = true;
            capacity_.remaining = 23 - length;
            std::memcpy(&internalString_[0], start, length);
        }
    }

    fString(const word word) :
        hash_(word.hash)
    {
        const std::size_t length = word.end - word.start;
        if (length > 23) {
            capacity_.isSmall = false;
            stringView_ = std::string_view(word.start, length);
        } else {
            capacity_.isSmall = true;
            capacity_.remaining = 23 - length;
            std::memcpy(&internalString_[0], word.start, length);
        }
    }

    template<std::size_t N>
    constexpr fString(const char (&str)[N]) :
        hash_(stringUtils::hash(str)),
        internalString_(),
        capacity_({
            .isSmall = true,
            .remaining = static_cast<char>(23 - (stringUtils::size(str)))
        })
    {
        stringUtils::copy(&str[0], &str[N - 1], &internalString_[0]);
    }

    constexpr auto size() const -> u64 {
        return isSmall() ? internalStringSize() : stringView_.length();
    }

    inline auto operator ==(const fString& other) const -> bool {
        if (size() != other.size()) {
            return false;
        } else if (capacity_.isSmall) {
            return std::memcmp(
                &internalString_,
                &other.internalString_,
                internalStringSize()) == 0;
        } else {
            return std::memcmp(
                &stringView_[0],
                &other.stringView_[0],
                stringView_.size()) == 0;
        }
    }

    inline auto operator !=(const fString& other) const -> bool {
        return !(*this == other);
    }

    friend auto operator <<(std::ostream& strm, const fString& fs) -> std::ostream& {
        return strm << 
            (fs.isSmall() 
                ? std::string_view(&fs.internalString_[0], fs.internalStringSize())
                : fs.stringView_);
    }
private:
    const std::size_t hash_;
    union {
        char internalString_[23];
        std::string_view stringView_;
    };
    struct smallCap {
        bool isSmall   : 1;
        char remaining : 7;
    } capacity_;

    constexpr auto internalStringSize() const -> u64 {
        return 23 - capacity_.remaining;
    }

    constexpr auto isSmall() const -> bool {
        return capacity_.isSmall;
    }
};

#pragma pack(pop)