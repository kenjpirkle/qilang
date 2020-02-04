#include "bucketArray.hpp"
#include <iostream>

auto main() -> int {
    bucketArray<int, 16> ba;
    ba.emplace(7);
    std::cout << ba.size() << '\n';
    return 0;
}