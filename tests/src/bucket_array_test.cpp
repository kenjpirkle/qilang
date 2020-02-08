#include "bucket_array.hpp"
#include "timer_utils.hpp"
#include <iostream>
#include <array>
#include <string>
#include <chrono>

template <int NB, int N>
static auto bucket_array_test() -> void {
    std::cout
        << "bucket_array_test\n"
        << "NB: " << NB << " N: " << N << '\n';

    bucket_array<std::string, NB> ba;
    std::array<std::string*, N> arr;

    timer_utils::bench(
        [&]() {
            for (int i = 0; i < N; ++i) {
                arr[i] = ba.emplace("hello " + std::to_string(i));
            }
        },
        "allocating"
    );

    timer_utils::bench(
        [&]() {
            for (int i = 0; i < N; i += 2) {
                ba.remove(arr[i]);
                arr[i] = nullptr;
            }
        },
        "removing"
    );

    timer_utils::bench(
        [&]() {
            for (int i = 0; i < N; i += 2) {
                arr[i] = ba.emplace("hello " + std::to_string(i));
            }
        },
        "inserting"
    );

    std::cout << "\n\n";
}