// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/10-delegating-constructors-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/10-delegating-constructors-1.cpp
//
// 教学要点:
//   1. 委托构造函数不能与其他成员初始化同时出现, 委托完成后再在函数体里赋值。
//   2. `Account(id_, name_, 0);` 这种语句只创建临时对象, 必须用初始化列表的语法
//      `: Account(id_, name_, 0) {}` 才是真正的委托。
//   3. 用成员初始化列表直接构造 obj, 避免多余的默认构造 + 移动赋值, 让计数 == 1.

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

struct Object { // 不要修改这个类的代码
    static int construction_counter;
    std::string name;
    Object() {
        construction_counter++;
    }

    Object(std::string name_) : name { name_ } {
        construction_counter++;
    }
};

class Account {
    std::string id;
    std::string name;
    std::string coin;
    Object obj;
public:

    // 委托给 (id, name) 版本, 不能再写其它成员初始化, 改在函数体内覆盖 coin
    Account(std::string id_)
        : Account(id_, "momo")
    {
        coin = "100元";
    }

    // 真正委托给 (id, name, int) 版本
    Account(std::string id_, std::string name_)
        : Account(id_, name_, 0)
    {
    }

    Account(std::string id_, std::string name_, int coin_)
        : id(id_), name(name_),
          coin(std::to_string(coin_) + "元"),
          obj(name_) // 在初始化列表里直接构造 obj, 仅 +1 次计数
    {
    }

    std::string get_id() const {
        return id;
    }

    std::string get_object_name() const {
        return obj.name;
    }

    std::string to_string() const {
        return "Account { id: " + id + ", name: " + name + ", coin: " + coin
            + ", Object { name: " + obj.name
            + ", construction_counter: " + std::to_string(Object::construction_counter) + " } }";
    }
};

int Object::construction_counter { 0 };

int main() { // 不要修改main函数中的代码

    Account a1 { "1111",  "hello" };
    std::cout << a1.to_string() << std::endl;
    d2x_assert(a1.get_id() == "1111");

    Object::construction_counter = 0;
    Account a2 { "2222",  "d2learn", 100 };
    std::cout << a2.to_string() << std::endl;

    d2x_assert(a2.get_object_name() == "d2learn");
    d2x_assert_eq(Object::construction_counter, 1);

    return 0;
}
