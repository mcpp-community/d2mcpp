// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/04-rvalue-references.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/04-rvalue-references.cpp
//

import std;
import d2x;


struct Object;
static Object * object_address = nullptr;
static int move_ctor_calls = 0;   // 移动构造被调用的次数, 供断言检查

struct Object {
    int data = 0;
    Object() {
        std::cout << "Object():" << this << std::endl;
        object_address = this;
    }
    Object(const Object&) { std::cout << "Object(const Object&):" << this << std::endl; }
    Object(Object&&) { ++move_ctor_calls; std::cout << "Object(Object&&):" << this << std::endl; }
    ~Object() { std::cout << "~Object():" << this << std::endl; }
};

int main() { // 关闭编译器优化
    {
        std::cout << "----> 临时对像 - 右值1" << std::endl;
        Object();
        std::cout << "----> 临时对像 - 右值2(具名对象 + std::move)" << std::endl;
        // 注意: 不能写 `Object obj = Object();` 来观察移动构造 ——
        // C++17 起 prvalue 直接初始化目标, 保证复制省略, 连
        // -fno-elide-constructors 也无法让那次移动发生。
        // 从具名对象 std::move 才是标准无关的观察方式。
        Object named;
        Object obj = std::move(named);
        (void)obj;

        std::cout << "--------代码可修改区域-开始--------" << std::endl;

        // 使用右值引用延长临时对象的生命周期, 并允许修改其值
        Object &&objRef = Object();

        std::cout << "--------代码可修改区域-结束--------" << std::endl;

        objRef.data = 1; // 修改被延长生命周期的临时对象的值(不要直接改动这行代码)
        std::cout << "objRef.data = " << objRef.data << " - " << &objRef << std::endl;
        d2x::check((&objRef == object_address), "(&objRef == object_address)");
        // 钉住移动构造确实发生过。教学漂移之所以能静默发生, 正是因为
        // 从前没有任何断言检查它 —— 输出少了一行, 没人发现。
        d2x::check((move_ctor_calls >= 1), "(move_ctor_calls >= 1)");
    }

    return 0;
}
