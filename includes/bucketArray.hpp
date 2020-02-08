#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <vector>

#pragma pack(push, 1)

template <typename T, int N>
struct bucketArray {
public:
    bucketArray() : 
        free_({
            .bucketIndex = 0,
            .index = -1
        }),
        count_(0)
    {
    }

    auto size() const -> size_t {
        return count_;
    }

    auto emplace(T&& element) -> T* {
        T* pos = nullptr;

        if (hasFree()) {
            auto& b = *buckets_[free_.bucketIndex];
            pos = b.emplace(std::forward<T>(element), free_.index);
            free_.index = -1;
        } else if (count_ == maxSize()) {
            // all buckets fully occupied, have to allocate a new one
            bucket& b = *buckets_.emplace_back(new bucket());
            pos = b.emplace(std::forward<T>(element), 0);
            free_.bucketIndex = buckets_.size() - 1;
            free_.index = getNextFree();
        } else {
            // find place to insert new element
            const auto bucket = *std::find_if(
                buckets_.rbegin(),
                buckets_.rend(),
                [](const auto& buck) { return !buck->occupancies.all(); }
            );

            uint ind = 0;
            while (bucket->occupancies[ind] != 0) {
                ++ind;
            }

            pos = bucket->emplace(std::forward<T>(element), ind);
        }
        ++count_;
        return pos;
    }

    auto remove(T* element) -> void {
        // find bucket element is stored
        free_.bucketIndex = 0;
        uint index = buckets_[0]->indexOf(element);

        while (index >= N) {
            ++free_.bucketIndex;
            index = buckets_[free_.bucketIndex]->indexOf(element);
        }
        buckets_[free_.bucketIndex]->removeAt(index);
        free_.index = index;
        --count_;
    }
private:
    struct bucket {
        std::bitset<N> occupancies;
        std::array<T, N> elements;

        constexpr auto maxSize() const -> int {
            return sizeof(T) * N;
        }

        inline auto emplace(T&& element, uint index) -> T* {
            occupancies.flip(index);
            elements[index] = element;
            return &elements[index];
        }

        inline auto indexOf(T* element) -> uint {
            return element - &elements[0];
        }

        inline auto remove(T* element) -> void {
            int bit = element - &elements[0];
            occupancies.flip(bit);
        }

        inline auto removeAt(uint index) -> void {
            occupancies.flip(index);
        }
    };

    struct freeSlot {
        int bucketIndex;
        int index;
    };

    inline auto maxSize() const -> size_t {
        return N * buckets_.size();
    }

    inline auto hasFree() const -> bool {
        return free_.index != -1;
    }

    constexpr auto getNextFree() -> int {
        return N > 1 ? 1 : -1;
    }

    std::vector<bucket*> buckets_;
    freeSlot free_;
    size_t count_;
};

#pragma pack(pop)