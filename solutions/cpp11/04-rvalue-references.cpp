// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/04-rvalue-references.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/04-rvalue-references.cpp
//

#include <d2x/cpp/common.hpp>

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
        (void)obj;

        std::cout << "--------代码可修改区域-开始--------" << std::endl;

        // 使用右值引用延长临时对象的生命周期, 并允许修改其值
        Object &&objRef = Object();

        std::cout << "--------代码可修改区域-结束--------" << std::endl;

        objRef.data = 1; // 修改被延长生命周期的临时对象的值(不要直接改动这行代码)
        std::cout << "objRef.data = " << objRef.data << " - " << &objRef << std::endl;
        d2x_assert((&objRef == object_address));
    }

    return 0;
}
