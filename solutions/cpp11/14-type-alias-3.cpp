// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/14-type-alias-3.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/14-type-alias-3.cpp
//

#include <d2x/cpp/common.hpp>
#include <type_traits>

// 别名模板必须定义在命名空间作用域(不能写在函数体内)
template <typename T>
using my_add_pointer_t = typename std::add_pointer<T>::type;

int main() {

    int c = 20;
    my_add_pointer_t<decltype(c)> ptr = &c;

    bool ok = std::is_same<my_add_pointer_t<int>, int*>::value;

    d2x_assert(ok);
    d2x_assert_eq(*ptr, 20);

    return 0;
}
