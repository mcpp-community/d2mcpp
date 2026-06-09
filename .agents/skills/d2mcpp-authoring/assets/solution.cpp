// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/<std>/NN-topic-K.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/<std>/NN-topic-K.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>

int main() {

    // 1.<已修复>: 说明保持与练习一致,但代码为正确版本
    double a = 1.1;

    d2x_assert_eq(a, 1.1);

    // 2.<已填空>
    double b = a;
    (void)b; // 用 (void) 抑制未使用告警,不要留下会触发 warning 的代码

    d2x_assert_eq(b, 1.1);

    // 注意: 参考答案中删除 D2X_WAIT,且所有检查点均通过

    return 0;
}
