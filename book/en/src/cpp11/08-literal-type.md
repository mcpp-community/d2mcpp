<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/08-literal-type.html
[English]: ./08-literal-type.html

# Literal Types

A *literal type* (LiteralType) is the C++11 named requirement for types that are allowed to participate in compile-time evaluation. It pairs with constexpr from ch07: constexpr decides "can this function or variable be evaluated at compile time", and *literal type* decides "what kinds of values are allowed to enter that compile-time world". C++11 also opened up **user-defined literals** (`42_km`, `"abc"_s`) so user types can be written with the same literal-style syntax as built-ins.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-LiteralType](https://en.cppreference.com/w/cpp/named_req/LiteralType) / [cppreference-user-literal](https://en.cppreference.com/w/cpp/language/user_literal) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/08-literal-type.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/08-literal-type-0.cpp) |  |


**Why was it introduced?**

- Together with constexpr, it lets *user-defined types* participate in compile-time evaluation, not just built-ins like int / double
- Provides a more readable literal syntax for units, measurements, and strong-typed integers (e.g. `1_km`, `3_sec`, `42ms`)
- Extends the range of code that can be evaluated at compile time from built-in scalars to domain types like Vector / Point / Color

**What makes a type a literal type?**

- Built-in scalar types (int / double / pointer / nullptr_t / enum, ...) are literal types automatically
- Arrays: an array of literal types is itself a literal type
- User-defined types: at least one constexpr constructor (other than copy/move) + a trivial or constexpr destructor + every non-static data member is a literal type + every base class is a literal type

## I. Basic Usage and Scenarios

### Built-in Type Literals

Built-in scalar types are literal types out of the box, and can directly participate in compile-time computation when combined with constexpr.

```cpp
constexpr char c = 'A';
constexpr int  a = 1;
constexpr double pi = 3.14;

constexpr int sum = a + 2 + 3;  // computed at compile time
```

### Turning a User Type Into a Literal Type (Add a constexpr Constructor)

A plain `Vector` class as written below is **not** a literal type, so it cannot be used in constexpr contexts.

```cpp
struct Vector {
    int x, y;
    Vector(int x_, int y_) : x(x_), y(y_) { } // ordinary constructor, not constexpr
};

constexpr Vector v{1, 2}; // error: Vector has no constexpr constructor
```

Marking the constructor `constexpr` upgrades `Vector` to a literal type, so it can be passed into constexpr functions and composed at compile time.

```cpp
struct Vector {
    int x, y;
    constexpr Vector(int x_, int y_) : x(x_), y(y_) { } // key: constexpr ctor
};

constexpr Vector add(const Vector& a, const Vector& b) {
    return Vector(a.x + b.x, a.y + b.y);
}

constexpr Vector v1{1, 2}, v2{2, 3};
constexpr Vector v3 = add(v1, v2); // {3, 5}, computed at compile time
```

### Literal Type + constexpr Function = Small Compile-Time Computations

Combining literal types with constexpr functions lets us move small chunks of "business logic" that used to live at runtime into compile time. Below, splitting a string into an array and summing it both happen at compile time.

```cpp
constexpr std::array<int, 3> to_array(const char *str) {
    return { str[0] - '0', str[1] - '0', str[2] - '0' };
}

constexpr auto arr = to_array("123");
constexpr int sum  = arr[0] + arr[1] + arr[2]; // 6 at compile time
```

### User-Defined Literals - operator"" _suffix

C++11 allows literals like `42_km` / `"abc"_s` by overloading `operator"" _suffix`. The suffix name **must start with an underscore** - suffixes without an underscore are reserved for the standard library.

```cpp
struct Length {
    long double meters;
};

// floating-point literal suffix: 1.5_km
constexpr Length operator"" _km(long double v) {
    return Length{ v * 1000.0L };
}

// integer literal suffix: 200_m
constexpr Length operator"" _m(unsigned long long v) {
    return Length{ static_cast<long double>(v) };
}

constexpr Length d1 = 1.5_km; // 1500 m
constexpr Length d2 = 200_m;  // 200 m
```

String literals can also have user-defined suffixes, e.g. a `_s` that builds a `std::string`.

```cpp
std::string operator"" _s(const char* str, std::size_t len) {
    return std::string(str, len);
}

auto greet = "hello"_s; // std::string
```

## II. Important Notes

### A User-Defined Literal Suffix Must Start With an Underscore

Suffixes without a leading underscore (e.g. `s`, `min`, `if`) are reserved for the standard library. A user-defined literal suffix without the underscore has undefined behavior, and compilers typically warn about it directly.

```cpp
// bad: no leading underscore
long double operator"" km(long double v); // warning / undefined

// good
long double operator"" _km(long double v); // ok
```

### "Literal Type" Is Not the Same as "Compile-Time Constant"

A literal type only says **the type itself is eligible to participate in constexpr** - it does not mean every value of that type is known at compile time. A plain `int` variable is a literal type, but its value could very well come from runtime input.

```cpp
int x;
std::cin >> x;        // value of x is determined at runtime
// int is still a literal type - type eligibility != value known
```

To actually make a value usable at compile time, mark the variable itself `constexpr` (or `const` with a compile-time initializer).

### Cooked Literals vs Raw Literals

User-defined literals come in two flavors.

- **cooked**: the compiler first parses the literal using the built-in rules and then passes the parsed value to your operator. This is the common case - `operator"" _km(long double)` receives an already-parsed floating-point value.
- **raw**: the compiler hands you the **original character sequence** of the literal and lets you parse it yourself. The signature is `operator"" _suf(const char* str)`, useful when you need to bypass the built-in parsing (e.g. a custom big-integer parser).

```cpp
// cooked: receives a long double
constexpr long double operator"" _km(long double v) { return v * 1000.0L; }

// raw: receives the literal "1500" as a string of characters
constexpr long long operator"" _bigint(const char* str) {
    long long n = 0;
    for (auto p = str; *p; ++p) n = n * 10 + (*p - '0');
    return n;
}
```

### Destructor: Trivial in C++11, Relaxed to constexpr in C++20

C++11 requires a literal type's destructor to be **trivial**. That's why `std::string` historically wasn't a literal type - it has to free heap memory in its destructor. C++20 relaxes this rule to "the destructor may be constexpr", which is why `std::string` can be used in constexpr contexts starting in C++20.

```cpp
// In C++11 / C++17, std::string is not a literal type
constexpr std::string s = "abc"; // error in C++17
```

### A constexpr Constructor Cannot Be Just a Copy/Move Constructor

LiteralType requires at least one constexpr constructor that is **not** a copy or move constructor. In other words, putting constexpr only on the copy constructor is not enough - there must be a constexpr constructor that can build the object from raw data.

## III. Practice Code

### Practice Topics

- 0 - [Understand the literal-type concept - what can be constexpr](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/08-literal-type-0.cpp)
- 1 - [Define your own literal type - add a constexpr constructor to Vector](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/08-literal-type-1.cpp)

### Practice Code Auto-detection Command

```
d2x checker literal-type-0
d2x checker literal-type-1
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
