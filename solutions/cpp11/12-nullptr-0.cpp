// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/12-nullptr-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/12-nullptr-0.cpp
//

#include <d2x/cpp/common.hpp>

#include <type_traits>
#include <cstddef>

int main() {

    // 1. 初始化指针
    int* ptr1 = nullptr; // 推荐用法
    int* ptr2 = NULL;   // 修复这里，添加正确类型
    int* ptr3 = 0;      // 不推荐的传统用法

    d2x_assert(ptr1 == nullptr);
    d2x_assert(ptr2 == nullptr);
    d2x_assert(ptr3 == nullptr);

    // 2. nullptr的类型
    bool ok = std::is_same<decltype(nullptr), std::nullptr_t>::value;
    d2x_assert(ok);

    // 3. 使用 nullptr 进行指针比较
    int value = 42;
    int* ptr4 = &value;

    if (ptr4 != nullptr) {
        *ptr4 = 2233;
        d2x_assert_eq(*ptr4, 2233);
    }

    // 4. 不同类型的指针都可以使用 nullptr
    double* dptr = nullptr;
    char* cptr = nullptr;
    void* vptr = nullptr;

    d2x_assert(dptr == nullptr);
    d2x_assert(cptr == nullptr);
    d2x_assert(vptr == nullptr);

    return 0;
}
