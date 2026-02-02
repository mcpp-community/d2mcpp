// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/12-nullptr-2.cpp
//
// Exercise/练习: cpp11 | 12 - nullptr | 指针字面量 - 模板编程
//
// Tips/提示: 使用 nullptr 在模板编程中提供更好的类型安全性
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

#include <d2x/cpp/common.hpp>
#include <type_traits>

// 模板函数示例
template<typename T>
void processPointer(T* ptr) { }

// 模板函数，返回传入值的副本
template<typename T>
T clone(const T& t) {
    return t;
}

int main() {

    // 1. nullptr 在模板中的类型推导
    auto ptr1 = nullptr;  // ptr1 的类型是 std::nullptr_t
    static_assert(std::is_same<decltype(ptr1), std::nullptr_t>::value, "类型推导错误");

    // 2. nullptr 在模板函数中的优势 - 类型推导安全
    // 观察编译器报错, 尝试显式指定模板参数解决报错
    processPointer<int>(clone<int *>(0));
    processPointer<char>(clone(NULL));

    processPointer<int>(clone(nullptr));
    processPointer<char>(clone(nullptr));

    D2X_WAIT

    return 0;
}
