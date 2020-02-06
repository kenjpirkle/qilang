#pragma once

#include "typeDefinitions.hpp"
#include <cstring>
#include <vector>
#include <string_view>

#pragma pack(push, 1)

// fString acts as a union of a small buffer optimized string or a string_view
struct fString {
public:
    inline auto size() const -> u64 {
        return isSmall() ? internalStringSize() : stringView_.length;
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
private:
    std::size_t hash_;
    union {
        char internalString_[23];
        std::string_view stringView_;
    };
    struct smallCap {
        bool isSmall   : 1;
        char remaining : 7;
    } capacity_;

    inline auto internalStringSize() const -> u64 {
        return 23 - capacity_.remaining;
    }

    inline auto isSmall() const -> bool {
        return capacity_.isSmall;
    }
};

#pragma pack(pop)