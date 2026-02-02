// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/11-inherited-constructors-0.cpp
//
// Exercise/练习: cpp11 | 11 - inherited constructors | 继承构造函数
//
// Tips/提示: 根据编译器的输出和报错信息, 添加符合要求的构造函数, 并了解继承构造函数的基本用法
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/using_declaration.html#Inheriting_constructors
//   - https://mcpp-community.github.io/d2mcpp/cpp11/11-inherited-constructors.html
//
// Auto-Checker/自动检测命令:
//
//   d2x checker inherited-constructors
//

#include <d2x/common.hpp>

#include <iostream>
#include <string>

class ObjectBase {
public:
    ObjectBase(int x) { std::cout << "ObjectBase::ObjectBase(int): " << x << std::endl; }
    ObjectBase(double x) { std::cout << "ObjectBase::ObjectBase(double): " << x << std::endl; }

    D2X_YOUR_ANSWER

    void info() const { std::cout << "ObjectBase: " << this << std::endl; }
};

class ObjectA : public ObjectBase {
public:
    ObjectA(int x) : ObjectBase(x) { std::cout << "ObjectA::ObjectA(int)" << std::endl; }
    ObjectA(double y) : ObjectBase(y) { std::cout << "ObjectA::ObjectA(double)" << std::endl; }

    D2X_YOUR_ANSWER

    void tips_a() const {
        std::cout << "ObjectA: add constructors to ObjectA" << std::endl;
    }
};

class ObjectB : public ObjectBase {
public:
    using ObjectBase::ObjectBase;

    // D2X_YOUR_ANSWER ?

    void tips_b() const {
        std::cout << "ObjectB: add new constructors to ObjectBase" << std::endl;
    }
};

int main() { // 不要直接修改 main 函数中的代码

    ObjectBase obj1(1), obj2(2.0), obj3 { 3, 4.0 };
    ObjectA a1(11), a2(22.0), a3 { 33, 44.0 };
    ObjectB b1(111), b2(222.0), b3 { 333, 444.0 };

    obj1.info();
    a1.info();
    b1.info();

    a1.tips_a();
    b1.tips_b();

    D2X_WAIT

    return 0;
}