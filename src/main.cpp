#include "fString.hpp"
#include "monoAllocator.hpp"
#include <iostream>
#include <array>
#include <string>

using namespace std::string_literals;

auto main() -> int {
    const std::array<fString, 10> fss = {
        "a","b","c","d","e","f","g","hello!","i","j"
    };
    auto fs2 = fString("hello!");

    for (const auto& f : fss) {
        std::cout
            << f
            << " and "
            << fs2
            << " are "
            << (f == fs2 ? "the same" : "not the same")
            << '\n';
    }
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