// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/15-variadic-templates-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/15-variadic-templates-1.cpp
//

#include <d2x/cpp/common.hpp>

// 递归终止: 只剩一个参数时直接返回
template<typename T>
T sum(T x) { return x; }

template<typename T, typename... Args>
T sum(T first, Args... args) {
    return first + sum(args...);
}

int main() {
    int res1 = sum(1, 2, 3, 4, 5);
    d2x_assert_eq(res1, 15);

    double res2 = sum(1.5, 2.5, 3.0);
    d2x_assert(res2 == 7.0);

    // 混合类型
    // 注意: 返回类型由第一个参数 T 决定
    int res3 = sum(10, 20.5);
    d2x_assert_eq(res3, 30);

    return 0;
}
