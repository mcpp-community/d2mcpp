// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp11/14-type-alias-0.cpp
//
// Exercise: cpp11 | 14 - type alias | Basic Type Aliases
//
// Tips: Use the using keyword to define type aliases, replacing traditional typedef
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

#include <d2x/common.hpp>

#include <type_traits>

int main() {

    // 1. Basic type alias definition
    D2X_YOUR_ANSWER Integer = int;
    D2X_YOUR_ANSWER = int;

    bool ok = std::is_same<Integer, int>::value; d2x_assert(ok);
    ok = std::is_same<Real, double>::value; d2x_assert(ok);

    // 2. Using type aliases
    Integer a = 42;
    Real b = 3.14;

    // 3. Verifying type aliases
    d2x_assert_eq(a, 42);
    d2x_assert_eq(b, 3.14);

    // 4. Type aliases are essentially the same
    int c = 100;
    Integer d = c;  // Can assign because they are essentially both int

    d2x_assert_eq(c, d);

    D2X_WAIT

    return 0;
}