// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/11-inherited-constructors-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/11-inherited-constructors-0.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

class ObjectBase {
public:
    ObjectBase(int x) { std::cout << "ObjectBase::ObjectBase(int): " << x << std::endl; }
    ObjectBase(double x) { std::cout << "ObjectBase::ObjectBase(double): " << x << std::endl; }

    // 新增 (int, double) 构造函数, 支持 ObjectBase obj3 { 3, 4.0 }
    ObjectBase(int x, double y) {
        std::cout << "ObjectBase::ObjectBase(int, double): " << x << ", " << y << std::endl;
    }

    void info() const { std::cout << "ObjectBase: " << this << std::endl; }
};

class ObjectA : public ObjectBase {
public:
    ObjectA(int x) : ObjectBase(x) { std::cout << "ObjectA::ObjectA(int)" << std::endl; }
    ObjectA(double y) : ObjectBase(y) { std::cout << "ObjectA::ObjectA(double)" << std::endl; }

    // 给 ObjectA 增加 (int, double) 构造函数, 转发给基类
    ObjectA(int x, double y) : ObjectBase(x, y) {
        std::cout << "ObjectA::ObjectA(int, double)" << std::endl;
    }

    void tips_a() const {
        std::cout << "ObjectA: add constructors to ObjectA" << std::endl;
    }
};

class ObjectB : public ObjectBase {
public:
    // ObjectB 通过继承构造函数自动获得 ObjectBase 的 (int, double) 等所有构造函数
    using ObjectBase::ObjectBase;

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
    (void)obj2; (void)obj3; (void)a2; (void)a3; (void)b2; (void)b3;

    return 0;
}
