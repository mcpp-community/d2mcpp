// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/04-rvalue-references.cpp
//
// Exercise/练习: cpp11 | 04 - rvalue references
//
// Tips/提示: 使用正确的方式来延长临时对象的生命周期, 让其能被再次修改
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/reference
//
// Auto-Checker/自动检测命令:
//
//   d2x checker rvalue-references
//

#include <d2x/common.hpp>

#include <iostream>
#include <string>

struct Object;
static Object * object_address = nullptr;

struct Object {
    int data = 0;
    Object() {
        std::cout << "Object():" << this << std::endl;
        object_address = this;
    }
    Object(const Object&) { std::cout << "Object(const Object&):" << this << std::endl; }
    Object(Object&&) { std::cout << "Object(Object&&):" << this << std::endl; }
    ~Object() { std::cout << "~Object():" << this << std::endl; }
};

int main() { // 关闭编译器优化
    {
        std::cout << "----> 临时对像 - 右值1" << std::endl;
        Object();
        std::cout << "----> 临时对像 - 右值2" << std::endl;
        Object obj = Object();

        std::cout << "--------代码可修改区域-开始--------" << std::endl;


        const Object &objRef = Object(); // 延长临时对象的生命周期


        std::cout << "--------代码可修改区域-结束--------" << std::endl;

        objRef.data = 1; // 修改被延长生命周期的临时对象的值(不要直接改动这行代码)
        std::cout << "objRef.data = " << objRef.data << " - " << &objRef << std::endl;
        d2x_assert((&objRef == object_address));
    }

    D2X_WAIT

    return 0;
}