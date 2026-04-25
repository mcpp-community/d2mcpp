// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/16-generalized-unions-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/16-generalized-unions-1.cpp
//

#include <d2x/cpp/common.hpp>
#include <vector>

union M
{
    int a1;
    std::vector<int> a2;
    M() {}
    M(const std::vector<int>& vec) : a2(vec) {
    }
    ~M() {}
};


int main() {

    M u1;
    u1.a1 = 21;

    // 1. 使用placement new构造
    new (&u1.a2) std::vector<int>();

    u1.a2 = {1, 42, 3};

    // 2. 验证：正常访问
    d2x_assert_eq(u1.a2[1], 42);

    // 3. 手动析构
    u1.a2.~vector();

    return 0;
}
