#pragma once

#include <memory>
#include <array>
#include <variant>
#include <vector>
#include <iostream>

using namespace std;

#pragma pack(push, 1)

template <typename T, int N, int SN = 0>
struct mono_allocator {
public:
    mono_allocator() {
        buffer_ = buffer_variant {
            in_place_index_t<1>{},
            large_buffer {
                .curr = nullptr,
                .last = nullptr,
                .slabs = {}
            }
        };
    }
    ~mono_allocator() {
        for (auto& s : large().slabs) {
            delete s;
        }
    }
    auto emplace_back(T&& element) -> T* {
        if (large().curr == large().last) {
            allocate_slab();
        }

        *large().curr = element;
        T* ptr = large().curr;
        ++large().curr;
        return ptr;
    }

private:
    using slab = array<T, N>;
    using small_size_slabs = array<slab, SN>;
    struct large_buffer {
        T* curr;
        T* last;
        vector<slab*> slabs;
    };
    using buffer_variant = variant<small_size_slabs, large_buffer>;

    buffer_variant buffer_;

    auto allocate_slab() -> void {
        auto s = large().slabs.emplace_back(new slab);
        large().curr = &((*s)[0]);
        large().last = &((*s)[N]);
    }

    inline auto large() -> large_buffer& {
        return get<1>(buffer_);
    }
};

#pragma pack(pop)