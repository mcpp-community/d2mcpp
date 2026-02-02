// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/10-delegating-constructors-1.cpp
//
// Exercise: cpp11 | 10 - delegating constructors | Delegating Constructors Precautions
//
// Tips: Fix compiler errors based on compiler output, understand precautions for delegating constructors
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/initializer_list.html#Delegating_constructor
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/10-delegating-constructors.md
//
// Auto-Checker command:
//
//   d2x checker delegating-constructors
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

struct Object { // Do not modify the code of this class
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

    Account(std::string id_)
        : Account(id_, "momo"), coin { "100元" }
    {

    }

    Account(std::string id_, std::string name_) {
        Account(id_, name_, 0);
    }

    Account(std::string id_, std::string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";
        obj = Object(name_);
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

int main() { // Do not modify the code in the main function

    Account a1 { "1111",  "hello" };
    std::cout << a1.to_string() << std::endl;
    d2x_assert(a1.get_id() == "1111");

    Object::construction_counter = 0;
    Account a2 { "2222",  "d2learn", 100 };
    std::cout << a2.to_string() << std::endl;

    d2x_assert(a2.get_object_name() == "d2learn");
    d2x_assert_eq(Object::construction_counter, 1);

    D2X_WAIT

    return 0;
}