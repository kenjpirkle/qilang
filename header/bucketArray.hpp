#pragma once

#include <array>
#include <bitset>

template <typename T, int N>
struct bucketArray {
public:
    auto size() const -> size_t {
        return count_;
    }
    auto emplace(T&& element) -> T& {
        if (count_ == maxSize()) {
            // have to allocate a new bucket
            allocNewBucket();
            last_[0] = element;
        }

        ++count_;
    }
private:
    struct bucket {
        std::bitset<N> occupancies;
        std::array<T, N> elements;
        bucket* next;

        constexpr auto maxSize() -> int {
            return sizeof(T) * N;
        }
    };

    auto maxSize() -> int {
        return N * bucketCount_;
    }

    inline auto allocNewBucket() -> void {
        last_->next = new bucket();
        last_ = last_->next;
        ++bucketCount_;
    }

    bucket* first_;
    bucket* last_;
    size_t count_;
    size_t bucketCount_;
};