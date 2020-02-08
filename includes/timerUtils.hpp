#pragma once

#include <chrono>
#include <iostream>
#include <functional>
#include <string>

namespace timerUtils {
    static inline auto bench(std::function<void()> f, const std::string& message) -> void {
        std::cout << message << '\n';
        auto start = std::chrono::steady_clock::now();
        f();
        auto end = std::chrono::steady_clock::now();
        std::cout
            << "finished: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << '\n';
    }   
}