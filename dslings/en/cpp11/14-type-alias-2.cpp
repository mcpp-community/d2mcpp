// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp11/14-type-alias-2.cpp
//
// Exercise: cpp11 | 14 - type alias | Alias Templates Basics
//
// Tips: Use alias templates to create aliases for template types
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/type_alias
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/14-type-alias.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker Command:
//
//   d2x checker type-alias
//

#include <d2x/cpp/common.hpp>

#include <vector>
#include <array>
#include <queue>

// 1. Basic alias template
template<typename T>
D2X_YOUR_ANSWER Vec = std::vector<T>;

// 2. Fixed-length alias template
template<typename T>
using Vec3 = std::array<T, 1>;

// 3. Alias template with default parameters
template<typename T, typename Compare>
using Heap = std::priority_queue<T, std::vector<T>, Compare>;

int main() {

    Vec<int> numbers = {1, 2, 3};
    Vec3<float> v3 = {1.0f, 2.0f, 3.0f};
    Heap<int> minHeap;

    d2x_assert_eq(numbers[0], 1);
    d2x_assert_eq(numbers[1], 2);
    d2x_assert_eq(numbers[2], 3);

    d2x_assert_eq(v3[0], 1.0f);
    d2x_assert_eq(v3[1], 2.0f);
    d2x_assert_eq(v3[2], 3.0f);

    D2X_WAIT

    return 0;
}