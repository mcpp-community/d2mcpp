// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/hello-mcpp.cpp
//
// Exercise/练习: 自动化代码练习使用教学
//
// Tips/提示:
//    该项目是使用xlings工具搭建的自动化代码练习项目, 通过在项目根目录下
//    执行 xlings checker 进入"编译器驱动开发模式"的练习代码自动检测.
//    你需要根据控制台的报错和提示信息, 修改代码中的错误. 当修复所有编译错误和
//    运行时检查点后, 你可以删除或注释掉代码中的 D2X_WAIT 宏, 会自动进入下一个练习.
//
//      - D2X_WAIT: 该宏用于隔离不同练习, 你可以删除或注释掉该宏, 进入下一个练习.
//      - d2x_assert_eq: 该宏用于运行时检查点, 你需要修复代码中的错误, 使得所有
//      - D2X_YOUR_ANSWER: 该宏用于提示你需要修改的代码, 一般用于代码填空(即用正确的代码替换这个宏)
//
// Docs/文档:
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/chapter_1.md
//   - book/src/chapter_1.md
//
// Auto-Checker/自动检测命令:
//
//   d2x checker hello-mcpp
//

#include <d2x/cpp/common.hpp>

// 修改代码时可以观察到控制台"实时"的变化

int main() {

    std::cout << "hello, mcpp!" << std:endl; // 0.修复这个编译错误

    int a = 1.1; // 1.修复这个运行时错误, 修改int为double, 通过检查

    d2x_assert_eq(a, 1.1); // 2.运行时检查点, 需要修复代码通过所有检查点(不能直接删除检查点代码)

    D2X_YOUR_ANSWER b = a; // 3.修复这个编译错误, 给b一个合适的类型

    d2x_assert_eq(b, 1); // 4.运行时检查点2

    D2X_WAIT // 5.删除或注释掉这个宏, 进入下一个练习(项目正式代码练习)

    return 0;
}

//// --- 更多详细介绍 | | |
//                  V V V
/*

# [[ 控制台输出解读 ]]

🌏Progress: [>----------] 0/10 -->> 显示当前的练习进度

[Target: 00-0-hello-mcpp] - normal -->> 当前的练习名

❌ Error: Compilation/Running failed for dslings/hello-mcpp.cpp -->> 显示检测状态

 The code exist some error!

---------C-Output--------- - 编译器输出信息
[HONLY LOGW]: main: dslings/hello-mcpp.cpp:24 - ❌ | a == 1.1 (1 == 1.100000) -->> 错误提示及位置(24行)
[HONLY LOGW]: main: dslings/hello-mcpp.cpp:26 - 🥳 Delete the D2X_WAIT to continue...


AI-Tips-Config: https://d2learn.org/docs/xlings -->> AI提示(需要配置大模型的key, 可不使用)

---------E-Files---------
dslings/hello-mcpp.cpp -->> 当前检测的文件
-------------------------

Homepage: https://github.com/d2learn/xlings

*/