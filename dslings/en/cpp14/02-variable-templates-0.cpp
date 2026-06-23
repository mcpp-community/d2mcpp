// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/en/cpp14/02-variable-templates-0.cpp
//
// Exercise: cpp14 | 02 - variable templates | basic variable templates
//
// Tips:
// - Variable templates use type parameters to control values
// - template<typename T> constexpr T name = ...;
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/variable_template
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/02-variable-templates.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker variable-templates
//

#include <d2x/cpp/common.hpp>

template <typename T>
constexpr D2X_YOUR_ANSWER pi = T(3.1415926535897932385);

template <typename T>
constexpr size_t max_buffer = D2X_YOUR_ANSWER;

template <>
constexpr size_t max_buffer<D2X_YOUR_ANSWER> = 4096;

int main() {

    d2x_assert_eq(pi<double>, 3.1415926535897932385);
    d2x_assert(pi<float> > 3.14f);
    d2x_assert(pi<float> < 3.142f);

    d2x_assert_eq(max_buffer<int>, 1024);
    d2x_assert_eq(max_buffer<char>, 1024);
    d2x_assert_eq(max_buffer<double>, D2X_YOUR_ANSWER);

    D2X_WAIT

    return 0;
}
