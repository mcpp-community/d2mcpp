<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/00-auto-and-decltype.html
[English]: ./00-auto-and-decltype.html

# Type Deduction - auto and decltype

auto and decltype are powerful **type deduction** tools introduced in C++11. They not only make code more concise but also enhance the expressive power of templates and generics.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-auto](https://en.cppreference.com/w/cpp/language/auto) / [cppreference-decltype](https://en.cppreference.com/w/cpp/language/decltype) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/00-auto-and-decltype.md) | [Video Explanation](https://www.bilibili.com/video/BV1EzJs6HEf7) / [Exercise Walkthrough](https://www.bilibili.com/video/BV1xkdYYUEyH) | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-0.cpp) |  |


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

```cpp
int a = 1;

auto b1 = a + 2;
decltype(a + 2 + 1.1) b2 = a + 2 + 1.1;

auto c1 = a + '0';
decltype(2 + 'a') c2 = 2 + 'a';
```

### Complex Type Deduction

**Iterator Type Deduction**

```cpp
std::vector<int> v = {1, 2, 3};

// std::vector<int>::iterator it = v.begin();
auto it = v.begin(); // Automatically deduce iterator type
// decltype(v.begin()) it = v.begin();
for (; it != v.end(); ++it) {
    if (*it == 2) {
        v.insert(it, 0);
        break;
    }
}
```

**Function Type Deduction**

> For complex types like functions or lambda expressions, auto and decltype are commonly used. Generally, lambda definitions use auto, while template type parameters use decltype.

```cpp
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

## II. Real-World Case - auto/decltype in the STL

> The examples above illustrate syntax; the practical value of auto/decltype is demonstrated most directly in the standard library's own implementation. The following picks two common pieces of code from the in-repo [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) to demonstrate `auto` and `decltype` respectively; `_STD` and the like are internal library macros and qualifiers — focus on the `auto` / `decltype` when reading.

### auto deduces iterator and element types: traversing a container

Traversing a container is the most common scenario for `auto`. Below is a traversal from path normalization in `<filesystem>`: `auto _Pos` deduces the iterator type, and `const auto _Elem` deduces the element type obtained by dereferencing — the very same form as `auto it = v.begin()` from "Complex type deduction - iterators" in `## I`.

```cpp
// MSVC STL · msvc-stl/stl/inc/filesystem (abridged, original indentation kept)
            auto _New_end = _Vec.begin();
            for (auto _Pos = _Vec.begin(); _Pos != _Vec.end();) {
                const auto _Elem = *_Pos++;
                // ...(decide whether to write _Elem back to _New_end; omitted)
            }
```

`_Vec` is the container of path components; both its iterator type and its element type are left to `auto`, with no need to spell out the concrete types.

### decltype takes the type of a variable: the binary search in std::lower_bound

The most direct use of `decltype` is "take the type of a variable or expression". In the standard library's binary search `std::lower_bound`, `auto` first deduces the range length `_Count`, then `decltype(_Count)` denotes "the same type as `_Count`" to convert `_Count / 2` back to that type:

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (abridged) —— std::lower_bound
    auto _UFirst = _STD _Get_unwrapped(_First);
    auto _Count  = _STD distance(_UFirst, _STD _Get_unwrapped(_Last));

    while (0 < _Count) { // divide and conquer, find half that contains answer
        const auto _Count2 = static_cast<decltype(_Count)>(_Count / 2);
        const auto _UMid   = _STD next(_UFirst, _Count2);
        // ...(compare at _UMid, narrow the range; omitted)
    }
```

This is the same use as `decltype(b) b2` from "Declaration and definition" in `## I`: `decltype(_Count)` is simply "the type of `_Count`". `auto` deduces the type, and `decltype` reuses that same type elsewhere.

> Takeaway: traversing a container, reusing the type of some variable — these everyday forms are, inside the standard library, exactly the auto + decltype toolkit taught in this chapter. This is one of the core motivations for introducing them in C++11.

## III. Important Notes

### auto and const / reference stripping

> auto deduction **strips top-level const and references**; to keep them you must write `const auto&` / `auto&` explicitly, whereas decltype preserves the declared type exactly

```cpp
int a = 1;
int &b = a;
const int c = 1;
const int &d = c;

auto a1 = a; // int
auto b1 = b; // int
auto c1 = c; // int
auto d1 = d; // int

const auto c2 = c;  // const int
const auto &d2 = d; // const int &

decltype(c) c3 = c; // const int
decltype(d) d3 = d; // const int &
```

This is also why `auto a = obj.a;` in "Class/Struct Member Type Deduction" yields `int` rather than `const int` — auto stripped the top-level const.

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

## IV. Practice Code

### Practice Topics

- 0 - [Declaration & Definition](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-0.cpp)
- 1 - [Expression Type Deduction](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-1.cpp)
- 2 - [Complex Type Deduction - Iterator / Function](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-2.cpp)
- 3 - [Function Return Type Deduction](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-3.cpp)
- 4 - [Class/Struct Member Type Deduction](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-4.cpp)
- 5 - [const & Reference Stripping and Preservation](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/00-auto-and-decltype-5.cpp)

### Auto-Checker Commands

<details>
<summary>Don't have d2x yet? Click to expand setup</summary>

```bash
# 1. Install xlings (Linux / macOS)
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash
# Windows PowerShell:
# irm https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.ps1 | iex

# 2. Install d2x and fetch this tutorial
xlings install d2x -y
d2x install d2mcpp

# 3. Enter the project directory & run the checker
cd d2mcpp
```

</details>

```
d2x checker auto-and-decltype
```

### Exercise Discussion

- [auto/decltype exercise thread](https://forum.d2learn.org/post/357)

## V. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [mcpp-community Organization](https://github.com/mcpp-community)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
- [Exercise Checker - d2x](https://github.com/d2learn/d2x)
