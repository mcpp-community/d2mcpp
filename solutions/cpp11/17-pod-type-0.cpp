// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/17-pod-type-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/17-pod-type-0.cpp
//

#include <d2x/cpp/common.hpp>

#include <type_traits>

struct A {
    int x;
    double y;
};

struct B {
    A a;
    int z;
};

struct C {
    virtual void foo() {} // 提供定义避免链接错误
    int x;
};

struct D {
    int x;
private:
    int y;
};

int main() {
    // 1. 判断 A/B 是否为 POD
    static_assert(std::is_pod<A>::value && std::is_pod<B>::value, "A and B should be POD");

    // 2. 判断 C/D 是否不是 POD
    static_assert(!std::is_pod<C>::value && !std::is_pod<D>::value, "C and D should not be POD");

    // 3. 判断 A 是否是 trivial /standard_layout 类型
    bool ok = std::is_trivial<A>::value && std::is_standard_layout<A>::value;
    d2x_assert(ok);

    // 4. 判断 B 是否是 trivial /standard_layout 类型
    ok = std::is_trivial<B>::value && std::is_standard_layout<B>::value;
    d2x_assert(ok);

    // 5. C 含虚函数 -> 不是 trivial 类型 (assert 期望条件成立, 因此用否定)
    ok = !std::is_trivial<C>::value;
    d2x_assert(ok);

    // 6. D 同时含 public/private 数据 -> 不是 standard_layout
    ok = std::is_standard_layout<D>::value;
    d2x_assert(!ok);
}
