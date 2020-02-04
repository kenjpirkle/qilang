#pragma once

#include <algorithm>
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
            bucket& b = *buckets_.emplace_back(new bucket());
            b.elements[0] = element;
            b.occupancies.set(0);
            ++count_;
            return b.elements[0];
        } else {
            // find place to insert new element
            const auto bucket = *std::find_if(
                buckets_.rbegin(),
                buckets_.rend(),
                [](const auto& buck) { return !buck->occupancies.all(); }
            );

            int slot = [&]() -> int {
                int s = 0;
                while (bucket->occupancies[s] != 0 || s < N) { ++s; }
                return s;
            }();

            bucket->elements[slot] = element;
            ++count_;
            return bucket->elements[slot];
        }
    }
private:
    struct bucket {
        std::bitset<N> occupancies;
        std::array<T, N> elements;

        constexpr auto maxSize() const -> int {
            return sizeof(T) * N;
        }
    };

    inline auto findSlot() -> T& {
        for (auto& bucket : buckets_) {
            if (bucket->occupancies.all() == false) {
                for (auto& slot : bucket) {
                    if (slot == 0) return bucket->elements[slot];
                }
            }
        }
    }

    inline auto maxSize() const -> size_t {
        return N * buckets_.size();
    }

    // TODO: implement union of bucket* & fully occupied bit
    std::vector<bucket*> buckets_;
    size_t count_;
};