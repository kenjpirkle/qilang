#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <vector>

template <typename T, int N>
struct bucketArray {
public:
    bucketArray() : free_(nullptr), count_(0) {
    }

    auto size() const -> size_t {
        return count_;
    }

    auto emplace(T&& element) -> T* {
        T* pos = nullptr;

        if (free_ != nullptr) {
            for (auto& buck : buckets_) {
                if (buck->contains(free_)) {
                    buck->emplace(element, free_);
                    pos = free_;
                    free_ = nullptr;
                    break;
                }
            }
        } if (count_ == maxSize()) {
            // all buckets fully occupied, have to allocate a new one
            bucket& b = *buckets_.emplace_back(new bucket());
            b.elements[0] = element;
            b.occupancies.set(0);
            free_ = &b.elements[1];
            pos = &b.elements[0];
        } else {
            // find place to insert new element
            const auto bucket = *std::find_if(
                buckets_.rbegin(),
                buckets_.rend(),
                [](const auto& buck) { return !buck->occupancies.all(); }
            );

            int slot = 0;
            while (bucket->occupancies[slot] != 0) {
                ++slot;
            }

            bucket->elements[slot] = element;
            bucket->occupancies.set(slot);
            pos = &bucket->elements[slot];
        }
        ++count_;
        return pos;
    }

    auto remove(T* element) -> void {
        // find bucket element is stored
        for (auto& buck : buckets_) {
            if (buck->contains(element)) {
                buck->remove(element);
                free_ = element;
                break;
            }
        }
        --count_;
    }
private:
    struct bucket {
        std::bitset<N> occupancies;
        std::array<T, N> elements;

        constexpr auto maxSize() const -> int {
            return sizeof(T) * N;
        }

        inline auto emplace(T& element, T* pos) -> void {
            int bit = (pos - &elements[0]) / sizeof(T);
            occupancies.flip(bit);
            elements[bit] = element;
        }

        inline auto contains(T* element) -> bool {
            return &elements[0] <= element && element < &elements[N];
        }

        inline auto at(T* element) -> int {
            return (element - &elements[0]) / sizeof(T);
        }

        inline auto remove(T* element) -> void {
            int bit = (element - &elements[0]) / sizeof(T);
            occupancies.flip(bit);
        }

        inline auto setOccupied(T* element) -> void {
            int bit = (element - &elements[0]) / sizeof(T);
            occupancies.flip(bit);
        }
    };

    inline auto maxSize() const -> size_t {
        return N * buckets_.size();
    }

    // TODO: implement union of bucket* & fully occupied bit
    std::vector<bucket*> buckets_;
    T* free_;
    size_t count_;
};