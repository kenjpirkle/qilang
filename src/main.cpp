#include "monoAllocator.hpp"
#include <iostream>
#include <array>
#include <string>

using namespace std::string_literals;

auto main() -> int {
    monoAllocator<std::string, 1024> ma;
    std::array<std::string*, 2048> arr;
    for (int i = 0; i < 2048; ++i) {
        arr[i] = ma.emplaceBack("hello " + std::to_string(i));
    }
    for (const auto& s : arr) {
        std::cout << *s << '\n';
    }
    return 0;
}