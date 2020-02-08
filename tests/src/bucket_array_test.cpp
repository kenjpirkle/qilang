#include "bucket_array.hpp"
#include "timer_utils.hpp"
#include <iostream>
#include <array>
#include <string>
#include <chrono>

using namespace std;

template <int NB, int N>
static auto bucket_array_test() -> void {
    cout
        << "bucket_array_test\n"
        << "NB: " << NB << " N: " << N << '\n';

    bucket_array<string, NB> ba;
    array<string*, N> arr;

    timer_utils::bench(
        [&]() {
            for (int i = 0; i < N; ++i) {
                arr[i] = ba.emplace("hello " + to_string(i));
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
                arr[i] = ba.emplace("hello " + to_string(i));
            }
        },
        "inserting"
    );

    cout << "\n\n";
}