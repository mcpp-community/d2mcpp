// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/16-generalized-unions-2.cpp
//
// Exercise/练习: cpp11 | 16 - generalized unions | 带标签的鉴别联合体
//
// Tips/提示:
// - 使用 enum 标记联合体中哪个成员是活跃的
// - 根据标签决定构造哪个成员、析构哪个成员
// - 联合体本身不知道哪个成员活跃, 标签由外层结构体维护
//
// Docs/文档:
//   - https://cppreference.com/w/cpp/language/union.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/16-generalized-unions.md
//
// 练习交流讨论: http://forum.d2learn.org/category/20
//
// Auto-Checker/自动检测命令:
//
//   d2x checker generalized-unions
//

#include <d2x/cpp/common.hpp>
#include <string>
#include <new>

// 标签类型 — 标记联合体中哪个成员是活跃的
enum class Tag {
    D2X_YOUR_ANSWER, // 整数活跃
    STRING           // 字符串活跃
};

// 联合体 — 包含 int 和 std::string, 需手动管理生命周期
union Data {
    int i;
    std::string s;

    Data() : i(0) {}
    ~Data() {} // 析构由外层根据 tag 决定
};

// 带标签的鉴别联合体
struct Value {
    Tag tag;
    Data data;

    // 构造 int
    Value(int val) : tag(D2X_YOUR_ANSWER) {
        new (&data.i) int(val);
    }

    // 构造 string
    Value(const std::string& val) : tag(Tag::STRING) {
        D2X_YOUR_ANSWER;
    }

    // 获取 int 成员 — 仅当 tag == INTEGER 时合法
    int as_int() {
        return data.i;
    }

    // 获取 string 成员 — 仅当 tag == STRING 时合法
    const std::string& as_string() {
        return data.s;
    }

    // 析构 — 根据 tag 决定析构哪个成员
    ~Value() {
        if (tag == Tag::STRING) {
            D2X_YOUR_ANSWER;
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

        d2x_assert_eq(v3.as_int(), D2X_YOUR_ANSWER);

        // 离开作用域时 ~Value() 发现 tag == INTEGER, 不析构 string
    }
    D2X_WAIT

    return 0;
}
