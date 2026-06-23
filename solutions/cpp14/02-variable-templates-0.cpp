// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/02-variable-templates-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/02-variable-templates-0.cpp
//

#include <d2x/cpp/common.hpp>

template <typename T>
constexpr T pi = T(3.1415926535897932385);

template <typename T>
constexpr size_t max_buffer = 1024;

template <>
constexpr size_t max_buffer<double> = 4096;

int main() {

    d2x_assert_eq(pi<double>, 3.1415926535897932385);
    d2x_assert(pi<float> > 3.14f);
    d2x_assert(pi<float> < 3.142f);

    d2x_assert_eq(max_buffer<int>, 1024);
    d2x_assert_eq(max_buffer<char>, 1024);
    d2x_assert_eq(max_buffer<double>, 4096);

    return 0;
}
