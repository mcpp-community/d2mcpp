// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/15-variadic-templates-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/15-variadic-templates-0.cpp
//

#include <d2x/cpp/common.hpp>
#include <sstream>

std::stringstream ss;

// 定义递归终止函数: 当参数包为空时调用
void print() {}

// 定义可变参数模板函数
template<typename T, typename... Args>
void print(T first, Args... args) {
    ss << first << " ";
    // 递归调用，处理剩余参数
    print(args...);
}

int main() {
    print(1, "hello", 3.14);

    std::string result = ss.str();
    d2x_assert(result == "1 hello 3.14 ");

    return 0;
}
