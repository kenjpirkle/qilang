#include "f_string.hpp"
#include "timer_utils.hpp"
#include "mono_allocator.hpp"
#include <array>
#include <iostream>
#include <string>

using namespace std;

template <int N, int S = 23>
auto f_string_test() -> void {
    cout
        << "f_string_test\n"
        << "S: " << S << '\n';

    mono_allocator<f_string<S>, N> ma;
    array<f_string<S>*, N> arr;

    timer_utils::bench(
        [&]() {
            for (int i = 0; i < N; ++i) {
                arr[i] = ma.emplace_back("hello " + to_string(i));
            }
        },
        "allocating"
    );

    for (const auto& f : arr) {
        cout << *f << '\n';
    }

    cout << "\n\n";
}