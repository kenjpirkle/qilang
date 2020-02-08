#include "monoAllocator.hpp"
#include "timerUtils.hpp"
#include <array>
#include <string>
#include <iostream>

template <int NB, int N> 
static auto monoAllocatorTest() -> void {
    std::cout
        << "monoAllocatorTest\n"
        << "NB: " << NB << " N: " << N << '\n';
    monoAllocator<std::string, NB> ma;
    std::array<std::string*, N> arr;

    timerUtils::bench(
        [&]() {
            for (int i = 0; i < N; ++i) {
                arr[i] = ma.emplaceBack("hello " + std::to_string(i));
            }
        },
        "allocating"
    );

    std::cout << "\n\n";
}