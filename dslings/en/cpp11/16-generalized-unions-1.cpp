// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp11/16-generalized-unions-1.cpp
//
// Exercise: cpp11 | 16 - generalized unions | generalized (non-trivial) unions
//
// Tips:
// - Allow unions to contain non-trivial types(non-POD)
//
// Docs:
//   - https://cppreference.com/w/cpp/language/union.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/16-generalized-unions.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker generalized-unions
//

#include <d2x/cpp/common.hpp>
#include <vector>

union M
{
    int a1;
    D2X_YOUR_ANSWER a2;
    M() {}
    M(const std::vector<int>& vec) : a2(vec) {
    }
    ~M() {}
};


int main() {

    M u1;
    u1.a1 = 21;

    // 1. Construct with placement new
    new (&u1.a2) std::vector<int>();

    u1.a2 = {1, D2X_YOUR_ANSWER, 3};

    // 2. use assert to verify the contents
    d2x_assert_eq(u1.a2[1], 42);

    // 3. Manually call the destructor
    u1.a2.~vector();

    D2X_WAIT

    return 0;
}
