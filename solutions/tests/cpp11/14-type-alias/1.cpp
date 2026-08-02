// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/14-type-alias-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/14-type-alias-1.cpp
//

import std;
import d2x;


static int func_called = 0;

// 函数声明
void example_func(int a, int b) {
    func_called = a + b;
}

int main() {

    // 1. 函数指针别名
    using FuncPtr = void(*)(int, int);

    FuncPtr func = example_func;

    func(1, 2);
    d2x::check_eq(func_called, 3, "func_called == 3");

    // 2. 容器类型别名
    using StringVector = std::vector<std::string>;

    // 使用容器类型别名
    StringVector strings = {"hello", "world"};

    // 3. 嵌套类型别名
    struct Container {
        using ValueType = int;
    };

    Container::ValueType value = 100;

    // 4. 验证类型别名
    d2x::check(strings[0] == "hello", "strings[0] == \"hello\"");
    d2x::check(strings[1] == "world", "strings[1] == \"world\"");
    d2x::check_eq(value, 100, "value == 100");

    return 0;
}
