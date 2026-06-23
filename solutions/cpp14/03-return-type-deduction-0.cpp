// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/03-return-type-deduction-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/03-return-type-deduction-0.cpp
//

#include <d2x/cpp/common.hpp>
#include <string>

auto get_forty_two() {
    return 42;
}

auto greet(const std::string& name) {
    return "hello " + name;
}

template <typename T1, typename T2>
auto max_of(T1 a, T2 b) {
    return a > b ? a : b;
}

int main() {

    d2x_assert_eq(get_forty_two(), 42);

    d2x_assert(greet(std::string("world")) == "hello world");

    d2x_assert_eq(max_of(10, 20), 20);
    d2x_assert_eq(max_of(2.5, 3.5), 3.5);

    return 0;
}
