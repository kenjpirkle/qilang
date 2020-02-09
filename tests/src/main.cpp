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

    std::cout << sizeof(mono_allocator<std::string, 70000, 50>) << '\n';
    mono_allocator_test<1, 70000, 70000>();
    std::cout << sizeof(mono_allocator<std::string, 70000, 1>) << '\n';
    mono_allocator_test<70000, 70000, 1>();
    std::cout << sizeof(mono_allocator<std::string, 2, 50>) << '\n';
    mono_allocator_test<1, 2, 50>();
    std::cout << sizeof(mono_allocator<std::string, 443, 2096>) << '\n';
    mono_allocator_test<16, 443, 2096>();
    std::cout << sizeof(mono_allocator<std::string, 444, 2048>) << '\n';
    mono_allocator_test<16, 444, 2048>();
    std::cout << sizeof(mono_allocator<std::string, 8192, 16>) << '\n';
    mono_allocator_test<1024, 8192, 16>();
    std::cout << sizeof(mono_allocator<std::string, 555, 1>) << '\n';
    mono_allocator_test<4096, 555, 1>();
    return 0;
}