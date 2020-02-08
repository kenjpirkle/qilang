#include "bucket_array_test.cpp"
#include "mono_allocator_test.cpp"
#include "mono_allocator.hpp"
#include <iostream>
#include <string>

auto main() -> int {
    bucket_array_test<1, 70000>();
    bucket_array_test<70000, 70000>();
    bucket_array_test<16, 444>();
    bucket_array_test<1024, 8192>();
    bucket_array_test<4096, 555>();
    mono_allocator_test<1, 70000>();
    mono_allocator_test<70000, 70000>();
    mono_allocator_test<1, 2>();
    mono_allocator_test<16, 443>();
    mono_allocator_test<16, 444>();
    mono_allocator_test<1024, 8192>();
    mono_allocator_test<4096, 555>();
    return 0;
}