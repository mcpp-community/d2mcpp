// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/<std>/tests/NN-topic/K.cpp
//
// Exercise/练习: <std> | NN - topic | <中文小标题>
//
// Tips/提示: <针对【本练习子主题】的一句话提示,需与本文件内容对应,避免「修复代码中的错误」这类通用话术>
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/...
//
// Auto-Checker/自动检测命令:
//
//   d2x checker <name>
//

import std;
import d2x;

int main() {

    // 1.<要修复的点>: <说明,例如:修复这个编译错误 / 改成正确类型>
    int a = 1.1; // <提示>

    d2x::check_eq(a, 1.1, "a == 1.1"); // 运行时检查点:需修复代码使其通过(不要删除检查点)

    // 2.<填空点>: 用正确的代码替换 D2X_YOUR_ANSWER(它不是宏,只是个没人定义的名字,
    //   编译器会指着它报错 —— 那就是要填的地方)
    D2X_YOUR_ANSWER b = a; // <提示:给 b 一个合适的类型>

    d2x::check_eq(b, 1, "b == 1");

    d2x::wait(); // 路障:读完本课删除这一行,进入下一个练习

    return 0;
}
