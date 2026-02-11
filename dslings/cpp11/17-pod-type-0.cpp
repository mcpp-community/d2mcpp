// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/17-pod-type-0.cpp
//
// Exercise/练习: cpp11 | 17 - POD Type | 基本类型性质判断
//
// Tips/提示:
//  - 使用 std::is_pod / std::is_trivial / std::is_standard_layout 判断类型性质
//  - 对比不同结构体定义对 POD 条件的影响
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/named_req/PODType
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/17-pod-type.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker pod-type

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
    virtual void foo();
    int x;
};

struct D {
    int x;
private:
    int y;
};

int main() {
    // TODO: 使用 static_assert 判断 A/B/C/D 是否是 POD 类型
    // 1. 判断 A/B 是否为 POD
    D2X_YOUR_ANSWER;

    // 2. 判断 C/D 是否不是 POD
    D2X_YOUR_ANSWER;

    // 3. 判断 A 是否是 trivial /standard_layout 类型，将结果写入 ok
    bool ok = D2X_YOUR_ANSWER;
    d2x_assert(ok);

    // 4. 判断 B 是否是 trivial /standard_layout 类型，将结果写入 ok
    ok = D2X_YOUR_ANSWER;
    d2x_assert(ok);

    // 5. 判断 C 是否是 trivial 类型，将结果写入 ok
    ok = D2X_YOUR_ANSWER;
    d2x_assert(ok);

    // 6. 判断 D 是否不是 standard_layout 类型，将结果写入 ok
    ok = D2X_YOUR_ANSWER;
    d2x_assert(!ok);

    D2X_WAIT
}
