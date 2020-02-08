#pragma once

#include <chrono>
#include <iostream>
#include <functional>
#include <string>

using namespace std;

namespace timer_utils {
    static inline auto bench(const function<void()> f, const string& message) -> void {
        auto start = chrono::steady_clock::now();
        f();
        auto end = chrono::steady_clock::now();
        cout << message << ": ";
        cout
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << '\n';
    }   
}