#pragma once

#include <cstring>
#include <vector>
#include "typeDefinitions.hpp"

#pragma pack(push, 1)

struct fString {
public:
private:
    std::size_t hash_;
    union {
        char internalString_[23];
        struct {
            const char* start;
            u64 length;
        } stringView_;
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