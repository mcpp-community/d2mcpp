// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/16-generalized-unions-2.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/16-generalized-unions-2.cpp
//

#include <d2x/cpp/common.hpp>
#include <string>
#include <new>

enum class Tag {
    INTEGER,
    STRING
};

union Data {
    int i;
    std::string s;

    Data() : i(0) {}
    ~Data() {}
};

struct Value {
    Tag tag;
    Data data;

    Value(int val) : tag(Tag::INTEGER) {
        new (&data.i) int(val);
    }

    Value(const std::string& val) : tag(Tag::STRING) {
        new (&data.s) std::string(val);
    }

    int as_int() {
        return data.i;
    }

    const std::string& as_string() {
        return data.s;
    }

    ~Value() {
        if (tag == Tag::STRING) {
            data.s.~basic_string();
        }
    }
};


int main() {

    // 1. 构造 int 值并验证
    Value v1(42);
    d2x_assert(v1.tag == Tag::INTEGER);
    d2x_assert_eq(v1.as_int(), 42);

    // 2. 构造 string 值并验证
    Value v2(std::string("hello"));
    d2x_assert(v2.tag == Tag::STRING);
    d2x_assert(v2.as_string() == "hello");

    // 3. 从 string 切换到 int
    {
        Value v3(std::string("world"));
        d2x_assert(v3.as_string() == "world");

        // 手动析构 string 成员, 切换到 int
        v3.data.s.~basic_string();
        v3.tag = Tag::INTEGER;
        v3.data.i = 100;

        d2x_assert_eq(v3.as_int(), 100);

        // 离开作用域时 ~Value() 发现 tag == INTEGER, 不析构 string
    }

    return 0;
}
