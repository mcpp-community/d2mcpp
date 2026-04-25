// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/09-list-initialization-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/09-list-initialization-1.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>

struct Object {
    Object() {
        x = 0;
        std::cout << "Object()" << std::endl;
    }
    Object(int x) : x(x) {
        std::cout << "Object(int): " << x << std::endl;
    }
    int x;
};

int main() {

    // `Object obj1();` 会被解析为函数声明 (most vexing parse)
    // 使用大括号初始化避免该陷阱
    Object obj1{};
    Object obj2(2);

    d2x_assert_eq(obj1.x, 0);
    d2x_assert_eq(obj2.x, 2);

    return 0;
}
