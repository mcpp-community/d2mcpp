// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/07-deprecated-attribute-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/07-deprecated-attribute-0.cpp
//

#include <d2x/cpp/common.hpp>

[[deprecated("Use new_add")]]
int old_add(int a, int b) { return a + b; }

int new_add(int a, int b) { return a + b; }

int main() {

    int r = old_add(10, 20);
    d2x_assert_eq(r, 30);

    return 0;
}
