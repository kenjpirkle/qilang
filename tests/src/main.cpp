#include "bucket_array_test.cpp"
#include "mono_allocator_test.cpp"
#include "f_string_test.cpp"
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

    mono_allocator_test<1, 70000, 70000>();
    mono_allocator_test<70000, 70000, 1>();
    mono_allocator_test<1, 2, 50>();
    mono_allocator_test<16, 443, 2096>();
    mono_allocator_test<16, 444, 2048>();
    mono_allocator_test<1024, 8192, 16>();
    mono_allocator_test<4096, 555, 1>();

    f_string_test<1>();
    f_string_test<2, 23>();
    f_string_test<3, 128>();
    f_string_test<4, 1024>();
    f_string_test<5, 2048>();
    return 0;
}