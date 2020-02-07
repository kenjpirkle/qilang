#pragma once

#include <memory>
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

        T* ptr = static_cast<T*>(curr_);
        curr_ += sizeof(T);
        *ptr = element;
        return ptr;
    }

private:
    constexpr static int slabSize_ = sizeof(T) * N;

    auto allocateSlab() -> void {
        curr_ = std::malloc(slabSize_);
        last_ = curr_ + slabSize_;
        slabs_.emplace_back(curr_);
    }

    void* curr_;
    void* last_;
    std::vector<void*> slabs_;
};

#pragma pack(pop)