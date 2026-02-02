// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/11-inherited-constructors-2.cpp
//
// Exercise/练习: cpp11 | 11 - inherited constructors | 继承构造函数在泛型装饰器的应用
//
// Tips/提示: 根据编译器的输出和报错信息, 模仿NoCopy类模板, 实现一个NoMove类模板
//           让被NoMove修饰的类, 创建出来的对象不支持移动语义
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

struct Point {
    double mX, mY;

    Point() : mX { 0 }, mY { 0 } { }
    Point(double x, double y) : mX { x }, mY { y } { }

    std::string to_string() const {
        return "{ " + std::to_string(mX)
            + ", " + std::to_string(mY) + " }";
    }
};

template <typename T>
class NoCopy : public T {
public:
    using T::T;

    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;
    NoCopy(NoCopy&&) = default;
    NoCopy& operator=(NoCopy&&) = default;
};

int main() {

    Point p1(1, 1);
    NoCopy<Point> p2(2, 2);

    std::cout << "p1: " << p1.to_string() << std::endl;
    std::cout << "p2: " << p2.to_string() << std::endl;

    auto p11 = p1;
    std::cout << "p11: " << p11.to_string() << std::endl;
    d2x_assert_eq(p1.mX, p11.mX);
    d2x_assert_eq(p1.mY, p11.mY);

    decltype(p2) p22 = p2; // by std::move?
    std::cout << "p22: " << p22.to_string() << std::endl;
    d2x_assert_eq(p2.mX, p22.mX);
    d2x_assert_eq(p2.mY, p22.mY);

    NoMove<Point> p3(3, 3);
    std::cout << "p3: " << p3.to_string() << std::endl;

    NoMove<Point> p33(0, 0);
    p33 = std::move(p3); // by copy?
    std::cout << "p33: " << p33.to_string() << std::endl;
    d2x_assert_eq(p3.mX, p33.mX);
    d2x_assert_eq(p3.mY, p33.mY);

    D2X_WAIT

    return 0;
}