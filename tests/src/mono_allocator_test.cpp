#include "mono_allocator.hpp"
#include "timer_utils.hpp"
#include <array>
#include <string>
#include <iostream>

using namespace std;

template <int NB, int N, int SN = 0> 
static auto mono_allocator_test() -> void {
    cout
        << "mono_allocator_test\n"
        << "NB: " << NB << " N: " << N << " SN: " << SN << '\n';
    mono_allocator<string, NB, SN> ma;
    array<string*, N> arr;

    timer_utils::bench(
        [&]() {
            for (int i = 0; i < N; ++i) {
                arr[i] = ma.emplace_back("hello " + to_string(i));
            }
        },
        "allocating"
    );

    cout << "\n\n";
}