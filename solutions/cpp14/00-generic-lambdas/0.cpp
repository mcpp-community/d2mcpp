// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/00-generic-lambdas-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/00-generic-lambdas-0.cpp
//

import std;
import d2x;

int main() {

    // 0. 简单泛型 lambda — identity
    auto identity = [](auto x) {
        return x;
    };

    d2x::check_eq(identity(42), 42, "identity(42) == 42");
    d2x::check(identity(std::string("hello")) == "hello", "identity(std::string(\"hello\")) == \"hello\"");
    d2x::check_eq(identity(3.14), 3.14, "identity(3.14) == 3.14");

    // 1. 泛型 lambda 做比较
    auto greater = [](auto a, auto b) {
        return a > b;
    };

    d2x::check(greater(5, 3), "greater(5, 3)");
    d2x::check(greater(2.5, 1.2), "greater(2.5, 1.2)");
    d2x::check(greater(std::string("z"), std::string("a")), "greater(std::string(\"z\"), std::string(\"a\"))");

    // 2. 推导类型确认
    auto get_type_size = [](auto x) {
        return sizeof(x);
    };

    d2x::check_eq(get_type_size(42), sizeof(int), "get_type_size(42) == sizeof(int)");
    d2x::check_eq(get_type_size('c'), sizeof(char), "get_type_size('c') == sizeof(char)");

    return 0;
}
