// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/12-nullptr-2.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/12-nullptr-2.cpp
//

#include <d2x/cpp/common.hpp>
#include <type_traits>

// 模板函数示例
template<typename T>
void processPointer(T* ptr) { (void)ptr; }

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
    // 显式指定模板参数, 避免 0/NULL 被推导为 int
    processPointer<int>(clone<int *>(0));
    processPointer<char>(clone<char *>(NULL));

    processPointer<int>(clone(nullptr));
    processPointer<char>(clone(nullptr));

    return 0;
}
