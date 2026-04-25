// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/01-default-and-delete-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/01-default-and-delete-1.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>

// 实现std::unique_ptr不可以拷贝, 但可以移动的属性
struct UniquePtr {
    void *dataPtr;
    UniquePtr() = default;

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&&) = default;
    UniquePtr& operator=(UniquePtr&&) = default;
};

int main() { // 不要直接修改main函数中的代码

    UniquePtr a;

    d2x_assert(std::is_copy_constructible<UniquePtr>::value == false);
    d2x_assert(std::is_copy_assignable<UniquePtr>::value == false);
    d2x_assert(std::is_move_constructible<UniquePtr>::value == true);
    d2x_assert(std::is_move_assignable<UniquePtr>::value == true);
    (void)a;

    return 0;
}
