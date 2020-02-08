#pragma once

#include <chrono>
#include <iostream>
#include <functional>
#include <string>

using namespace std;

namespace timer_utils {
    static inline auto bench(function<void()> f, const string& message) -> void {
        cout << message << ": ";
        auto start = chrono::steady_clock::now();
        f();
        auto end = chrono::steady_clock::now();
        cout
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << '\n';
    }   
}