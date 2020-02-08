#include "bucketArray.hpp"
#include <iostream>
#include <array>
#include <string>

static auto bucketArrayTest() -> void {
    constexpr int NUM = 8192;
    bucketArray<std::string, 1> ba;
    std::array<std::string*, NUM> arr;
    for (int i = 0; i < NUM; ++i) {
        arr[i] = ba.emplace("hello " + std::to_string(i));
    }
    for (const auto& s : arr) {
        std::cout << *s << '\n';
    }

    std::cout << "count: " << ba.size() << '\n';

    for (int i = 0; i < NUM; i += 2) {
        ba.remove(arr[i]);
        arr[i] = nullptr;
    }
    for (const auto& s : arr) {
        if (s != nullptr) {
            std::cout << *s << '\n';
        }
    }

    std::cout << "count: " << ba.size() << '\n';

    for (int i = 0; i < NUM; i += 2) {
        arr[i] = ba.emplace("hello " + std::to_string(i));
    }
    for (const auto& s : arr) {
        std::cout << *s << '\n';
    }

    std::cout << "count: " << ba.size() << '\n';
}