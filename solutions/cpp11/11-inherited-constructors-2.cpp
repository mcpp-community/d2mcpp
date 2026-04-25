// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/11-inherited-constructors-2.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/11-inherited-constructors-2.cpp
//
// 教学要点: NoMove 的拷贝控制只声明 = default 的拷贝构造/拷贝赋值, 不显式声明
// 移动操作 -> 移动操作不会被隐式生成, `std::move(p3)` 会回退到拷贝.

#include <d2x/cpp/common.hpp>

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

template <typename T>
class NoMove : public T {
public:
    using T::T;

    // 仅显式声明拷贝, 不声明移动 -> std::move 会回退到拷贝构造/拷贝赋值
    NoMove(const NoMove&) = default;
    NoMove& operator=(const NoMove&) = default;
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

    decltype(p2) p22 = std::move(p2); // NoCopy 不能拷贝, 只能 move
    std::cout << "p22: " << p22.to_string() << std::endl;
    d2x_assert_eq(p2.mX, p22.mX);
    d2x_assert_eq(p2.mY, p22.mY);

    NoMove<Point> p3(3, 3);
    std::cout << "p3: " << p3.to_string() << std::endl;

    NoMove<Point> p33(0, 0);
    p33 = std::move(p3); // NoMove 没有 move 重载, 回退到拷贝赋值
    std::cout << "p33: " << p33.to_string() << std::endl;
    d2x_assert_eq(p3.mX, p33.mX);
    d2x_assert_eq(p3.mY, p33.mY);

    return 0;
}
