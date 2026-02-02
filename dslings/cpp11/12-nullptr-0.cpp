// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/12-nullptr-0.cpp
//
// Exercise/练习: cpp11 | 12 - nullptr | 指针字面量 - 基础用法
//
// Tips/提示: 使用 nullptr 替代传统的 NULL 和 0 来初始化指针
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/nullptr
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/12-nullptr.md
//
// 练习交流讨论: http://forum.d2learn.org
//
// Auto-Checker/自动检测命令:
//
//   d2x checker nullptr
//

#include <d2x/common.hpp>

#include <type_traits>

int main() {

    // 1. 初始化指针
    int* ptr1 = nullpt; // 推荐用法
    int* ptr2 = NULL;   // 修复这里，添加正确类型
    int* ptr3 = 0;      // 不推荐的传统用法

    d2x_assert(ptr1 == nullptr);
    d2x_assert(ptr2 == nullptr);
    d2x_assert(ptr3 == nullptr);

    // 2. nullptr的类型
    bool ok = std::is_same<decltype(nullptr), D2X_YOUR_ANSWER>::value;
    d2x_assert(ok);

    // 3. 使用 nullptr 进行指针比较
    int value = 42;
    int* ptr4 = &value;

    if (ptr4 != nullptr) {
        *ptr4 = D2X_YOUR_ANSWER;
        d2x_assert_eq(*ptr4, 2233);
    }

    // 4. 不同类型的指针都可以使用 nullptr
    double* dptr = nullptr;
    char* cptr = nullptr
    void* vptr = nullptr;

    d2x_assert(dptr == nullptr);
    d2x_assert(cptr == nullptr);
    d2x_assert(vptr == nullptr);

    D2X_WAIT

    return 0;
}
