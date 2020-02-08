#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <vector>

using namespace std;

#pragma pack(push, 1)

template <typename T, int N>
struct bucket_array {
public:
    bucket_array() : 
        free_({
            .bucket_index = 0,
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

        if (has_free()) {
            auto& b = *buckets_[free_.bucket_index];
            pos = b.emplace(forward<T>(element), free_.index);
            free_.index = -1;
        } else if (count_ == max_size()) {
            // all buckets fully occupied, have to allocate a new one
            bucket& b = *buckets_.emplace_back(new bucket());
            pos = b.emplace(forward<T>(element), 0);
            free_.bucket_index = buckets_.size() - 1;
            free_.index = get_next_free();
        } else {
            // find place to insert new element
            const auto bucket = *find_if(
                buckets_.rbegin(),
                buckets_.rend(),
                [](const auto& buck) { return !buck->occupancies.all(); }
            );

            uint ind = 0;
            while (bucket->occupancies[ind] != 0) {
                ++ind;
            }

            pos = bucket->emplace(forward<T>(element), ind);
        }
        ++count_;
        return pos;
    }

    auto remove(T* element) -> void {
        // find bucket element is stored
        free_.bucket_index = 0;
        uint index = buckets_[0]->index_of(element);

        while (index >= N) {
            ++free_.bucket_index;
            index = buckets_[free_.bucket_index]->index_of(element);
        }
        buckets_[free_.bucket_index]->remove_at(index);
        free_.index = index;
        --count_;
    }
private:
    struct bucket {
        bitset<N> occupancies;
        array<T, N> elements;

        constexpr auto max_size() const -> int {
            return sizeof(T) * N;
        }

        inline auto emplace(T&& element, uint index) -> T* {
            occupancies.flip(index);
            elements[index] = element;
            return &elements[index];
        }

        inline auto index_of(T* element) -> uint {
            return element - &elements[0];
        }

        inline auto remove(T* element) -> void {
            int bit = element - &elements[0];
            occupancies.flip(bit);
        }

        inline auto remove_at(uint index) -> void {
            occupancies.flip(index);
        }
    };

    struct freeSlot {
        int bucket_index;
        int index;
    };

    inline auto max_size() const -> size_t {
        return N * buckets_.size();
    }

    inline auto has_free() const -> bool {
        return free_.index != -1;
    }

    constexpr auto get_next_free() -> int {
        return N > 1 ? 1 : -1;
    }

    vector<bucket*> buckets_;
    freeSlot free_;
    size_t count_;
};

#pragma pack(pop)