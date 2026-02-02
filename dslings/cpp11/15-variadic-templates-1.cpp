// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/15-variadic-templates-1.cpp
//
// Exercise/练习: cpp11 | 15 - variadic templates | 可变参数模板求和
//
// Tips/提示: 
// - 利用递归思想实现求和函数 sum
// - 同样需要处理递归终止条件
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/parameter_pack
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/15-variadic-templates.md
//

#include <d2x/common.hpp>

D2X_YOUR_ANSWER

template<typename T, D2X_YOUR_ANSWER>
T sum(T first, D2X_YOUR_ANSWER) {
    return first + sum(D2X_YOUR_ANSWER);
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

    D2X_WAIT

    return 0;
}
