<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/00-auto-and-decltype.html
[English]: ./00-auto-and-decltype.html

# Type Deduction - auto and decltype

auto and decltype are powerful **type deduction** tools introduced in C++11. They not only make code more concise but also enhance the expressive power of templates and generics.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-auto](https://en.cppreference.com/w/cpp/language/auto) / [cppreference-decltype](https://en.cppreference.com/w/cpp/language/decltype) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/00-auto-and-decltype.md) | [Video Explanation](https://www.bilibili.com/video/BV1xkdYYUEyH) | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-0.cpp) |  |


**Why were they introduced?**

- Solve the problem of overly complex type declarations
- Need to obtain object or expression types in template applications
- Support lambda expression definitions

**What's the difference between auto and decltype?**

- auto is often used for variable definitions, and the deduced type may lose const or reference (can be explicitly specified with auto &)
- decltype obtains the **exact type** of an expression
- auto generally cannot be used as a template type parameter

## I. Basic Usage and Scenarios

### Declaration and Definition

> Acts as a type placeholder to assist in variable definition or declaration. When using auto, the variable must be initialized, while decltype can be used without initialization.

```cpp
int b = 2;
auto b1 = b;
decltype(b) b2 = b;
decltype(b) b3; // Can be used without initialization
```

### Expression Type Deduction

> Often used for complex expression type deduction to ensure calculation precision

```c++
int a = 1;

auto b1 = a + 2;
decltype(a + 2 + 1.1) b2 = a + 2 + 1.1;

auto c1 = a + '0';
decltype(2 + 'a') c2 = 2 + 'a';
```

### Complex Type Deduction

**Iterator Type Deduction**

```c++
std::vector<int> v = {1, 2, 3};

auto it = v.begin(); // Automatically deduce iterator type
// decltype(v.begin()) it = v.begin();
for (; it != v.end(); ++it) {
    std::cout << *it << " ";
}
```

**Function Type Deduction**

> For complex types like functions or lambda expressions, auto and decltype are commonly used. Generally, lambda definitions use auto, while template type parameters use decltype.

```c++
int add_func(int a, int b) {
    return a + b;
}

int main() {
    auto minus_func = [](int a, int b) { return a - b; };

    std::vector<std::function<decltype(add_func)>> funcVec = {
        add_func,
        minus_func
    };

    funcVec[0](1, 2);
    funcVec[1](1, 2);
    //...
}
```

### Function Return Type Deduction

**Syntax Sugar Usage**

> auto supports trailing return type function definitions and can be used with decltype for return type deduction.

```cpp
auto main() -> int {
    return 0;
}

auto add(int a, double b) -> decltype(a + b) {
    return a + b;
}
```

**Function Template Return Type Deduction**

> When the template return type cannot be determined, auto + decltype can be used for deduction, allowing add to support general types like int, double,... and complex types like Point, Vec,... enhancing generic programming expressiveness. (In C++14, decltype can be omitted)

```cpp
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}
```

### Class/Structure Member Type Deduction

```cpp
struct Object {
    const int a;
    double b;
    Object() : a(1), b(2.0) { }
};

int main() {
    const Object obj;

    auto a = obj.a;
    std::vector<decltype(obj.b)> vec;
}
```

## II. Important Notes - The Impact of Parentheses

### Difference between decltype(obj) and decltype( (obj) )

- Generally, `decltype(obj)` obtains its declared type
- While `decltype( (obj) )` obtains the type of the `(obj)` expression (lvalue expression)

```cpp
int a = 1;
decltype(a) b; // Deduction result is a's declared type int
decltype( (a) ) c; // Deduction result is the type of (a) lvalue expression int &
```

### Difference between decltype(obj.b) and decltype( (obj.b) )

- `decltype( (obj.b) )`: Type deduction from expression perspective, obj's definition type affects deduction result. For example, if obj is const-qualified, const will limit obj.b access to const.
- `decltype(obj.b)`: Since it deduces the member's declared type, it won't be affected by obj's definition.

```cpp
struct Object {
    const int a;
    double b;
    Object() : a(1), b(2.0) { }
};

int main() {
    Object obj;
    const Object obj1;

    decltype(obj.b)  // double
    decltype(obj1.b) // double

    decltype( (obj.b) ) // double &
    decltype( (obj1.b) ) // Affected by obj1's const qualification, so it's const double &
}
```

### Rvalue Reference Variables are Lvalues in Expressions

```cpp
int &&b = 1;

decltype(b) // Deduction result is declared type int &&
decltype( (b) ) // Deduction result is int &
```

## III. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
