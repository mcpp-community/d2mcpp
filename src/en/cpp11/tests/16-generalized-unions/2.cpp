// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: src/en/cpp11/tests/16-generalized-unions/2.cpp
//
// Exercise: cpp11 | 16 - generalized unions | tagged discriminated union
//
// Tips:
// - Use an enum to tag which member of the union is active
// - Choose constructor/destructor based on the tag
// - The union itself cannot track which member is active; the tag is maintained externally
//
// Docs:
//   - https://cppreference.com/w/cpp/language/union.html
//   - https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/16-generalized-unions.md
//
// Discussion Forum: http://forum.d2learn.org/category/20
//
// Auto-Checker:
//
//   d2x checker generalized-unions
//

import std;
import d2x;

// Tag type — marks which member of the union is active
enum class Tag {
    D2X_YOUR_ANSWER,// integer is active
    STRING           // string is active
};

// Union — contains int and std::string, lifecycle managed externally
union Data {
    int i;
    std::string s;

    Data() : i(0) {}
    ~Data() {} // destruction decided by outer struct based on tag
};

// Tagged discriminated union
struct Value {
    Tag tag;
    Data data;

    // Construct with int
    Value(int val) : tag(D2X_YOUR_ANSWER) {
        new (&data.i) int(val);
    }

    // Construct with string
    Value(const std::string& val) : tag(Tag::STRING) {
        D2X_YOUR_ANSWER;
    }

    // Access int member — valid only when tag == INTEGER
    int as_int() {
        return data.i;
    }

    // Access string member — valid only when tag == STRING
    const std::string& as_string() {
        return data.s;
    }

    // Destructor — destroy the active member based on tag
    ~Value() {
        if (tag == Tag::STRING) {
            D2X_YOUR_ANSWER;
        }
    }
};


int main() {

    // 1. Construct an int value and verify
    Value v1(42);
    d2x::check(v1.tag == Tag::INTEGER, "v1.tag == Tag::INTEGER");
    d2x::check_eq(v1.as_int(), 42, "v1.as_int() == 42");

    // 2. Construct a string value and verify
    Value v2(std::string("hello"));
    d2x::check(v2.tag == Tag::STRING, "v2.tag == Tag::STRING");
    d2x::check(v2.as_string() == "hello", "v2.as_string() == \"hello\"");

    // 3. Switch from string to int
    {
        Value v3(std::string("world"));
        d2x::check(v3.as_string() == "world", "v3.as_string() == \"world\"");

        // Manually destroy the string member, then switch to int
        v3.data.s.~basic_string();
        v3.tag = Tag::INTEGER;
        v3.data.i = 100;

        d2x::check_eq(v3.as_int(), D2X_YOUR_ANSWER, "v3.as_int() == D2X_YOUR_ANSWER");

        // When v3 goes out of scope, ~Value() sees tag == INTEGER, skips string destruction
    }    
    d2x::wait();
    return 0;
}
