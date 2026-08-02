// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/intro/tests/hello-mcpp.cpp (参考答案)

import std;
import d2x;

int main() {

    std::cout << "hello, mcpp!" << std::endl; // 0.修复这个编译错误

    double a = 1.1; // 1.修复这个运行时错误, 修改int为double, 通过检查

    d2x::check_eq(a, 1.1, "a == 1.1"); // 2.运行时检查点, 需要修复代码通过所有检查点(不能直接删除检查点代码)

    int b = a; // 3.修复这个编译错误, 给b一个合适的类型(int 截断 1.1 -> 1)

    d2x::check_eq(b, 1, "b == 1"); // 4.运行时检查点2

    return 0;
}
