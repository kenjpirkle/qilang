#include "monoAllocator.hpp"
#include <array>
#include <string>
#include <iostream>

static auto monoAllocatorTest() -> void {
    constexpr int NUM = 16;
    monoAllocator<std::string, 2> ma;
    std::array<std::string*, NUM> arr;
    for (int i = 0; i < NUM; ++i) {
        arr[i] = ma.emplaceBack("hello " + std::to_string(i));
    }
    for (const auto& s : arr) {
        std::cout << *s << '\n';
    }
}