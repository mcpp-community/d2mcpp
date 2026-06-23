// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp14/08-aggregate-nsdmi-0.cpp
//
// Exercise: cpp14 | 08 - aggregate NSDMI | aggregate + default member init
//
// Tips:
// - C++14 allows classes with NSDMI to remain aggregates
// - Values provided in aggregate init override defaults; missing values
//   use NSDMI defaults
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/aggregate_initialization
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/08-aggregate-nsdmi.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker aggregate-nsdmi
//

#include <d2x/cpp/common.hpp>

struct Vec3 {
    float x = 0.0f;
    D2X_YOUR_ANSWER y = 0.0f;
    float z = 0.0f;
};

int main() {

    Vec3 v1{1.0f, 2.0f, 3.0f};
    d2x_assert_eq(v1.x, 1.0f);
    d2x_assert_eq(v1.z, 3.0f);

    Vec3 v2{4.0f, D2X_YOUR_ANSWER};
    d2x_assert_eq(v2.x, 4.0f);
    d2x_assert_eq(v2.y, 5.0f);
    d2x_assert_eq(v2.z, D2X_YOUR_ANSWER);

    D2X_WAIT

    return 0;
}
