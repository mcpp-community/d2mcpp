// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/14-type-alias-2.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/14-type-alias-2.cpp
//

#include <d2x/cpp/common.hpp>

#include <vector>
#include <array>
#include <queue>
#include <functional>

// 1. 基本别名模板
template<typename T>
using Vec = std::vector<T>;

// 2. 固定长度的别名模板
template<typename T>
using Vec3 = std::array<T, 3>;

// 3. 带默认参数的别名模板
template<typename T, typename Compare = std::less<T>>
using Heap = std::priority_queue<T, std::vector<T>, Compare>;

int main() {

    Vec<int> numbers = {1, 2, 3};
    Vec3<float> v3 = {1.0f, 2.0f, 3.0f};
    Heap<int> minHeap;
    (void)minHeap;

    d2x_assert_eq(numbers[0], 1);
    d2x_assert_eq(numbers[1], 2);
    d2x_assert_eq(numbers[2], 3);

    d2x_assert_eq(v3[0], 1.0f);
    d2x_assert_eq(v3[1], 2.0f);
    d2x_assert_eq(v3[2], 3.0f);

    return 0;
}
