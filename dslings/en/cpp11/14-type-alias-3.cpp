// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp11/14-type-alias-3.cpp
//
// Exercise: cpp11 | 14 - type alias | Standard Library Style Alias Templates
//
// Tips: Use alias templates to implement standard library style _t/_v templates
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

    template <typename T>
    D2X_YOUR_ANSWER my_add_pointer_t = typename std::add_pointer<T>;

    int c = 20;
    my_add_pointer_t<decltype(c)> ptr = &c;

    bool ok = std::is_same<my_add_pointer_t<int>, int*>::value;

    d2x_assert(ok);
    d2x_assert_eq(*ptr, 20);

    D2X_WAIT

    return 0;
}