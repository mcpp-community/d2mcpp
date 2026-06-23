// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp14/07-deprecated-attribute-0.cpp
//
// Exercise: cpp14 | 07 - deprecated attribute | [[deprecated]]
//
// Tips:
// - [[deprecated("message")]] marks deprecated functions/variables
// - The compiler produces a warning on use
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/attributes
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/07-deprecated-attribute.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker deprecated-attribute
//

#include <d2x/cpp/common.hpp>

[[deprecated("Use new_add")]]
D2X_YOUR_ANSWER old_add(int a, int b) { return a + b; }

int new_add(int a, int b) { return a + b; }

int main() {

    int r = old_add(10, 20);
    d2x_assert_eq(r, D2X_YOUR_ANSWER);

    D2X_WAIT

    return 0;
}
