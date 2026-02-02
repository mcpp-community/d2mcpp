// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/15-variadic-templates-0.cpp
//
// Exercise/练习: cpp11 | 15 - variadic templates | 可变参数模板基础
//
// Tips/提示: 
// - 可变参数模板允许函数接受任意数量的参数
// - C++11中通常通过递归方式处理参数包
// - 需要定义一个递归终止函数（Base case）
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/parameter_pack
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/15-variadic-templates.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker variadic-templates
//

#include <d2x/cpp/common.hpp>
#include <sstream>

std::stringstream ss;

// 定义递归终止函数
// 当参数包为空时调用此函数
D2X_YOUR_ANSWER

// 定义可变参数模板函数
template<typename T,D2X_YOUR_ANSWER>
void print(T first, D2X_YOUR_ANSWER args) {
    ss << first << " ";
    // 递归调用，处理剩余参数
    print(D2X_YOUR_ANSWER);
}

int main() {
    print(1, "hello", 3.14);
    
    std::string result = ss.str();
    d2x_assert(result == "1 hello 3.14 ");

    D2X_WAIT

    return 0;
}
