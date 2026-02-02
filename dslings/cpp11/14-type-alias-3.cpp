// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/14-type-alias-3.cpp
//
// Exercise/练习: cpp11 | 14 - type alias | 标准库中的别名模板应用
//
// Tips/提示: 使用别名模板实现标准库风格的_t/_v模板
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/type_alias
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/14-type-alias.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker type-alias
//

#include <d2x/cpp/common.hpp>
#include <type_traits>

int main() {

    template <typename T>
    D2X_YOUR_ANSWER my_add_pointer_t = typename std::add_pointer<T>;

    int c = 20;
    my_add_pointer_t<decltype(c)> ptr = &c;

    bool ok = std::is_same<my_add_pointer_t<int>, int*>::value;

    d2x_assert(ok);
    d2x_assert_eq(*ptr, 20);

    D2X_WAIT

    return 0;
}
