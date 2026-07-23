// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/intro/tests/hello-mcpp.cpp
//
// Exercise/练习: 自动化代码练习使用教学
//
// Tips/提示:
//    这是一个「练习即测试」的自动化代码练习项目。两种使用方式任选:
//
//      d2x checker              引导模式: 自动检测,通过后进入下一题
//      mcpp test -p src/intro       原生模式: 直接使用 mcpp 运行, 测试报告即学习进度
//
//    你需要根据控制台的报错和提示信息修改代码。约定只有三个:
//
//      - D2X_YOUR_ANSWER: 填空占位符 —— 用正确的代码替换它。它不是宏,
//        只是一个没人定义的名字, 所以编译器会指着它报错, 那就是要填的地方
//      - d2x::check / d2x::check_eq: 运行时检查点, 修复代码让所有检查通过
//        (不能直接删除检查点)
//      - d2x::wait(): 练习之间的路障 —— 读完这一课, 删掉它才算真正完成
//
// Docs/文档:
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/chapter_1.md
//   - book/src/chapter_1.md
//
// Auto-Checker/自动检测命令:
//
//   d2x checker hello-mcpp
//

import std;
import d2x;

// 修改代码时可以观察到控制台"实时"的变化

int main() {

    std::cout << "hello, mcpp!" << std:endl; // 0.修复这个编译错误

    int a = 1.1; // 1.修复这个运行时错误, 修改int为double, 通过检查

    d2x::check_eq(a, 1.1, "a == 1.1"); // 2.运行时检查点, 需要修复代码通过所有检查点(不能直接删除检查点代码)

    int b = a; // 3.修复这个编译错误, 给b一个合适的类型

    d2x::check_eq(b, 1, "b == 1"); // 4.运行时检查点2

    d2x::wait(); // 5.删除或注释掉这一行, 进入下一个练习(项目正式代码练习)

    return 0;
}

//// --- 更多详细介绍 | | |
//                  V V V
/*

# [[ 控制台输出解读 ]]

🌏Progress: [>----------] 0/52 -->> 显示当前的练习进度

[Exercise: hello-mcpp] -->> 当前的练习名

❌ Error: Compilation/Running failed for src/intro/tests/hello-mcpp.cpp -->> 显示检测状态

---------Output--------- - 编译/运行输出信息
❌ | a == 1.1 (1 == 1.1)  --> src/intro/tests/hello-mcpp.cpp:41 -->> 错误提示及位置(41行)
🚧 | Delete the d2x::wait() to continue  --> src/intro/tests/hello-mcpp.cpp:47

AI-Tips-Config: https://xlings.d2learn.org/documents/d2x/intro.html -->> AI提示(需要配置大模型的key, 可不使用)

---------E-Files---------
src/intro/tests/hello-mcpp.cpp -->> 当前检测的文件
-------------------------

Homepage: https://github.com/openxlings/xlings

*/
