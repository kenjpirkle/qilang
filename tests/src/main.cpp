#include "bucketArrayTest.cpp"
#include "monoAllocatorTest.cpp"

auto main() -> int {
    bucketArrayTest<1, 70000>();
    bucketArrayTest<70000, 70000>();
    bucketArrayTest<16, 444>();
    monoAllocatorTest<1, 70000>();
    monoAllocatorTest<70000, 70000>();
    monoAllocatorTest<16, 444>();
    return 0;
}