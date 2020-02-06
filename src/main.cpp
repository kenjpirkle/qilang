#include "fString.hpp"
#include "monoAllocator.hpp"
#include "bucketArray.hpp"
#include <iostream>
#include <array>
#include <string>

auto main() -> int {
    // monoAllocator<std::string, 1024> ma;
    // std::array<std::string*, 8192> arr;
    // for (int i = 0; i < 8192; ++i) {
    //     arr[i] = ma.emplaceBack("hello " + std::to_string(i));
    // }
    // for (const auto& s : arr) {
    //     std::cout << *s << '\n';
    // }
    constexpr int NUM = 32;
    bucketArray<std::string, 16> ba;
    std::array<std::string*, NUM> arr;
    for (int i = 0; i < NUM; ++i) {
        arr[i] = ba.emplace("hello " + std::to_string(i));
    }
    for (const auto& s : arr) {
        std::cout << *s << '\n';
    }

    for (int i = 0; i < NUM; i += 2) {
        ba.remove(arr[i]);
        arr[i] = nullptr;
    }
    for (const auto& s : arr) {
        if (s != nullptr) {
            std::cout << *s << '\n';
        }
    }

    return 0;
}