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
    constexpr mono_allocator() {
        if constexpr (SN == 0) {
            // no stack buffer
            curr_ = last_ = nullptr;
        } else {
            // in place stack buffer optimization
            curr_ = &stack_buffer_[0];
            last_ = &stack_buffer_[SN * N];
        }
    }
    ~mono_allocator() {
        for (auto& s : heap_buffer_) {
            delete s;
        }
    }

    auto emplace_back(T&& element) -> T* {
        if (curr_ == last_) {
            allocate_slab();
        }

        *curr_ = element;
        T* ptr = curr_;
        ++curr_;
        return ptr;
    }

private:
    using slab = array<T, N>;

    T* curr_;
    T* last_;
    array<T, SN * N> stack_buffer_;
    vector<slab*> heap_buffer_;

    auto allocate_slab() -> void {
        auto s = heap_buffer_.emplace_back(new slab);
        curr_ = &((*s)[0]);
        last_ = &((*s)[N]);
    }
};

#pragma pack(pop)