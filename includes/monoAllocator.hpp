#pragma once

#include <memory>
#include <array>
#include <vector>

#pragma pack(push, 1)

template <typename T, int N>
struct monoAllocator {
public:
    monoAllocator() {
        curr_ = last_ = nullptr;
    }
    ~monoAllocator() {
        for (auto& s : slabs_) {
            std::free(s);
        }
    }
    auto emplaceBack(T&& element) -> T* {
        if (curr_ == last_) {
            allocateSlab();
        }

        *curr_ = element;
        T* ptr = curr_;
        ++curr_;
        return ptr;
    }

private:
    using slab = std::array<T, N>;

    auto allocateSlab() -> void {
        auto s = slabs_.emplace_back(new slab);
        curr_ = &((*s)[0]);
        last_ = &((*s)[N]);
    }

    T* curr_;
    T* last_;
    std::vector<slab*> slabs_;
};

#pragma pack(pop)