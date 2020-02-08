#include "bucketArray.hpp"
#include "timerUtils.hpp"
#include <iostream>
#include <array>
#include <string>
#include <chrono>

template <int NB, int N>
static auto bucketArrayTest() -> void {
    std::cout
        << "bucketArrayTest\n"
        << "NB: " << NB << " N: " << N << '\n';

    bucketArray<std::string, NB> ba;
    std::array<std::string*, N> arr;

    timerUtils::bench(
        [&]() {
            for (int i = 0; i < N; ++i) {
                arr[i] = ba.emplace("hello " + std::to_string(i));
            }
        },
        "allocating"
    );

    timerUtils::bench(
        [&]() {
            for (int i = 0; i < N; i += 2) {
                ba.remove(arr[i]);
                arr[i] = nullptr;
            }
        },
        "removing"
    );

    timerUtils::bench(
        [&]() {
            for (int i = 0; i < N; i += 2) {
                arr[i] = ba.emplace("hello " + std::to_string(i));
            }
        },
        "inserting"
    );

    std::cout << "\n\n";
}