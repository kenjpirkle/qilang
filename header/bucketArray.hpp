#pragma once

#include <array>
#include <bitset>
#include <vector>

template <typename T, int N>
struct bucketArray {
public:
    auto size() const -> size_t {
        return count_;
    }
    auto emplace(T&& element) -> T& {
        if (count_ == maxSize()) {
            // all buckets fully occupied, have to allocate a new one
            allocNewBucket();
            last_->elements[0] = element;
            last_->occupancies[0] = 1;
            ++count;
            return last_->elements[0];
        } else {
            // find place to insert new element
        }
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

    inline auto maxSize() -> int {
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