// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/01-default-and-delete-1.cpp
//
// Exercise/练习: cpp11 | 01 - default and delete | 不可拷贝对像
//
// Tips/提示: 根据编译器提示使用`= default`和`= delete`修复错误
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/function#Function_definition
//   - https://en.cppreference.com/w/cpp/language/function#Deleted_functions
//
// Auto-Checker/自动检测命令:
//
//   d2x checker default-and-delete-1
//

#include <d2x/cpp/common.hpp>

#include <iostream>

// 实现std::unique_ptr不可以拷贝, 但可以移动的属性
struct UniquePtr {
    void *dataPtr;
    UniquePtr() = default;
};

int main() { // 不要直接修改main函数中的代码

    // std::unique_ptr<int> a(new int(1));
    UniquePtr a;

    // 对像不可拷贝/复制
    // std::unique_ptr<int> b = a; // error
    d2x_assert(std::is_copy_constructible<UniquePtr>::value == false);
    // a = b; // error
    d2x_assert(std::is_copy_assignable<UniquePtr>::value == false);

    // 对像可移动
    // std::unique_ptr<int> c = std::move(a); // ok
    d2x_assert(std::is_move_constructible<UniquePtr>::value == true);
    // a = std::move(c); // ok
    d2x_assert(std::is_move_assignable<UniquePtr>::value == true);

    D2X_WAIT

    return 0;
}