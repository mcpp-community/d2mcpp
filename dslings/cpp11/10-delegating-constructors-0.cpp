// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/10-delegating-constructors-0.cpp
//
// Exercise/练习: cpp11 | 10 - delegating constructors | 委托构造函数
//
// Tips/提示: 根据编译器的输出, 修复编译器报错, 了解委托构造函数的基本使用
//
// Docs/文档:
//   - https://en.cppreference.com/w/cpp/language/initializer_list.html#Delegating_constructor
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/10-delegating-constructors.md
//
// Auto-Checker/自动检测命令:
//
//   d2x checker delegating-constructors
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

static int construction_counter { 0 };

class Account {
    std::string id;
    std::string name;
    std::string coin;
public:

    Account(std::string id_) {
        id = id_;
        name = "momo";
        coin = "0元";

        D2X_DONT_DELETE_THIS(construction_counter++);
    }

    Account(std::string id_, std::string name_) {
        id = id_;
        name = name_;
        coin = "0元";

        D2X_DONT_DELETE_THIS(construction_counter++);
    }

    Account(std::string id_, std::string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";

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

    D2X_WAIT

    return 0;
}