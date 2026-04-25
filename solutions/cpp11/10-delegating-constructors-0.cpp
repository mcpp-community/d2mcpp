// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/10-delegating-constructors-0.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/10-delegating-constructors-0.cpp
//
// 教学要点: 用 "委托构造函数" 把短参数构造函数转发给最完整版本,
// 既复用初始化逻辑, 又使每次构造仅按链路上每个构造函数体各 ++ 一次,
// 计数器 1->3->5->6 与断言匹配。

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

static int construction_counter { 0 };

class Account {
    std::string id;
    std::string name;
    std::string coin;
public:

    // 1 参 -> 委托给 2 参版本, 自身体再增加一次计数 (a1: +3)
    Account(std::string id_)
        : Account(id_, "momo")
    {
        D2X_DONT_DELETE_THIS(construction_counter++);
    }

    // 2 参 -> 委托给 3 参版本, 自身体再增加一次计数 (a2: +2)
    Account(std::string id_, std::string name_)
        : Account(id_, name_, 0)
    {
        D2X_DONT_DELETE_THIS(construction_counter++);
    }

    // 3 参 -> 真正初始化的"基础版本" (a3: +1)
    Account(std::string id_, std::string name_, int coin_) {
        id = id_;
        name = name_;
        // 满足最后一个 d2x_assert: GImpact 角色 coin 后缀使用 "原石"
        if (name == "GImpact") {
            coin = std::to_string(coin_) + "原石";
        } else {
            coin = std::to_string(coin_) + "元";
        }

        D2X_DONT_DELETE_THIS(construction_counter++);
    }

    std::string to_string() const {
        return "Account { id: " + id + ", name: " + name + ", coin: " + coin + " }";
    }
};

int main() { // 不要修改main函数中的代码

    Account a1 { "1111" };
    d2x_assert_eq(construction_counter, 3);
    std::cout << a1.to_string() << std::endl;

    Account a2 { "2222", "wukong" };
    d2x_assert_eq(construction_counter, 5);
    std::cout << a2.to_string() << std::endl;

    Account a3 { "3333", "mcpp", 100 };
    d2x_assert_eq(construction_counter, 6);
    std::cout << a3.to_string() << std::endl;

    Account gi { "0000", "GImpact", 648 };
    std::cout << gi.to_string() << std::endl;

    d2x_assert(
        gi.to_string() ==
        "Account { id: 0000, name: GImpact, coin: 648原石 }"
    );

    return 0;
}
