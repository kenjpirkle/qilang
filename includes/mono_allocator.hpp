#pragma once

#include <memory>
#include <array>
#include <vector>

using namespace std;

#pragma pack(push, 1)

template <typename T, int N>
struct mono_allocator {
public:
    mono_allocator() {
        curr_ = last_ = nullptr;
    }
    ~mono_allocator() {
        for (auto& s : slabs_) {
            free(s);
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

    auto allocate_slab() -> void {
        auto s = slabs_.emplace_back(new slab);
        curr_ = &((*s)[0]);
        last_ = &((*s)[N]);
    }

    T* curr_;
    T* last_;
    vector<slab*> slabs_;
};

#pragma pack(pop)