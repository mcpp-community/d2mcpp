// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/14-type-alias-2.cpp
//
// Exercise/练习: cpp11 | 14 - type alias | 别名模板基础
//
// Tips/提示: 使用别名模板为模板类型创建别名
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/type_alias
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/14-type-alias.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker type-alias
//

#include <d2x/common.hpp>

#include <vector>
#include <array>
#include <queue>

// 1. 基本别名模板
template<typename T>
D2X_YOUR_ANSWER Vec = std::vector<T>;

// 2. 固定长度的别名模板
template<typename T>
using Vec3 = std::array<T, 1>;

// 3. 带默认参数的别名模板
template<typename T, typename Compare>
using Heap = std::priority_queue<T, std::vector<T>, Compare>;

int main() {

    Vec<int> numbers = {1, 2, 3};
    Vec3<float> v3 = {1.0f, 2.0f, 3.0f};
    Heap<int> minHeap;

    d2x_assert_eq(numbers[0], 1);
    d2x_assert_eq(numbers[1], 2);
    d2x_assert_eq(numbers[2], 3);

    d2x_assert_eq(v3[0], 1.0f);
    d2x_assert_eq(v3[1], 2.0f);
    d2x_assert_eq(v3[2], 3.0f);

    D2X_WAIT

    return 0;
}
