// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp14/08-aggregate-nsdmi-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp14/08-aggregate-nsdmi-0.cpp
//

#include <d2x/cpp/common.hpp>

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

int main() {

    Vec3 v1{1.0f, 2.0f, 3.0f};
    d2x_assert_eq(v1.x, 1.0f);
    d2x_assert_eq(v1.z, 3.0f);

    Vec3 v2{4.0f, 5.0f};
    d2x_assert_eq(v2.x, 4.0f);
    d2x_assert_eq(v2.y, 5.0f);
    d2x_assert_eq(v2.z, 0.0f);

    return 0;
}
