// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/01-default-and-delete-2.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/01-default-and-delete-2.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>

void func(int x) {
    std::cout << "x = " << x << std::endl;
}

// 显式删除float参数的重载
void func(float) = delete;

int main() {

    func(1);     // int
    // func(1.1f);  // float - 已被delete, 删除该错误调用

    return 0;
}
