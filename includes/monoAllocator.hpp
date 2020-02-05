#pragma once

#include <memory>
#include <vector>

template <typename T, int N>
struct monoAllocator {
public:
    monoAllocator() {
        allocateSlab();
    }
    ~monoAllocator() {
        for (auto& s : slabs_) {
            std::free(s);
        }
    }
    auto emplace_back(T&& element) -> T& {
        if (curr_ == last_) {
            allocateSlab();
        }

        return *static_cast<T*>(curr_);
    }

private:
    constexpr int slabSize_ = sizeof(T) * N;

    auto allocateSlab() -> void {
        curr_ = std::malloc(slabSize_);
        last_ = curr_ + slabSize_ - sizeof(T);
        slabs_.emplace_back(curr_);
    }

    T* curr_;
    T* last_;
    std::vector<void*> slabs_;
};