<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/14-type-alias.html
[English]: ./14-type-alias.html

# Type alias and alias template

Type alias and alias template are important features introduced in C++11, used to create new names for existing types, enhancing the expressive power of generic programming, and improving code readability and maintainability.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-type-alias](https://en.cppreference.com/w/cpp/language/type_alias) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/14-type-alias.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/14-type-alias-0.cpp) |  |

> Note: The `using` keyword existed before C++11, but was mainly used for namespace and class member declarations
> - Declaring namespaces: `using namespace std;`
> - Class member declarations: `struct B : A { using A::member; };`

**Why introduced?**

- Replace traditional `typedef` syntax with a more intuitive way to define type aliases
- Support template aliases, enhancing the expressive power of generic programming
- Improve code readability, especially for complex types
- Consistent with `using` declaration syntax

**What's the difference between type alias and typedef?**

- More intuitive syntax: `using NewType = OldType;` vs `typedef OldType NewType;`
- Support template aliases, while `typedef` does not
- More flexible and powerful in template programming

## I. Basic Usage and Scenarios

### Basic Type Alias

> Create new names for existing types to improve code readability, and can replace traditional `typedef` alias definitions

```cpp
typedef int Integer; // Traditional typedef way
using Integer = int; // C++11 using way

// Using aliases
Integer i = 1;
int j = 2;
```

Type alias is not a new type, but an alias for other composite types, essentially the same. In the above code, the essence of `Integer` is `int`, commonly used to simplify type names.

### Complex Type Alias

> Create aliases for complex types (such as function pointers, nested types)

```cpp
// Function pointer alias
using FuncPtr = void(*)(int, int);
using StringVector = std::vector<std::string>;

// Nested type alias
struct Container {
    using ValueType = int;
    using Iterator = std::vector<ValueType>::iterator;
};

void example(int a, int b) {
    // Function implementation
}

int main() {
    FuncPtr func = example; // Equivalent: void(*func)(int, int) = example;
    StringVector strings = {"hello", "world"}; // Equivalent: std::vector<std::string> strings...
    Container::ValueType value = 100; // Equivalent: int value = 100;
    return 0;
}
```

For code like `void (*func)(int, int) = example;`, many people might hesitate before understanding it defines a function pointer. By using `using` to give complex types a type alias `FuncPtr`, using `FuncPtr func = example;` allows people to quickly understand the code's intent.

### Alias Template

> Create aliases for template types, enhancing generic programming capabilities

```cpp
// Alias template
template <typename T>
using Vec = std::vector<T>;

// Create "subset" alias types based on generics
template <typename T>
using Vec3 = std::array<T, 3>;
template <typename T>
using Vec4 = std::array<T, 4>;

// Alias template with default parameters
template <typename T, typename Compare = std::less<T>>
using Heap = std::priority_queue<T, std::vector<T>, Compare>;

int main() {
    Vec<int> numbers = {1, 2, 3};
    Vec3<float> v3 = {1.0f, 2.0f, 3.0f};
    Vec4<float> v4 = {1.0f, 2.0f, 3.0f, 4.0f};
    Heap<int> minHeap;
    Heap<int, std::greater<int>> maxHeap;
    return 0;
}
```

In addition to creating aliases for complex types, it also supports creating aliases for template types, and through template parameters, it can control the parameters/properties of the original template type - default parameters, allocator types, length, comparators, etc. In the above code, we created dynamic Vec type aliases; also created fixed-length `Vec3`, `Vec4` type aliases for special scenarios (vector, matrix calculations) by specifying length; and used template parameter defaults to create `Heap` type, using `vector` as the underlying data structure by default, supporting default min-heap, and setting max-heap by specifying template parameters.

### Standard Library _t Style Templates

> In STL, some templates provide _t versions to save the process of manually obtaining types and values. Type aliases can easily implement them. _v style suuport by `c++ 17` [inline variables + variable templates]

`Reference implementation of std::remove_const_t`

```cpp
// Implementation and principle explanation of remove_const can refer to: https://zhuanlan.zhihu.com/p/352972564
template <typename T>
using my_remove_const_t = typename std::remove_const<T>::type;

int main() {
    const int a = 10;
    my_remove_const_t<decltype(a)> b = a; // b's type is int, not const int
    return 0;
}
```

## II. Precautions

### Alias is Not a New Type

> Type alias is just a synonym for existing types and does not create new types

```cpp
using MyInt = int;
using YourInt = int;

int main() {
    MyInt a = 10;
    YourInt b = 20;

    a = b;  // Can assign because both are int types
    static_assert(std::is_same<MyInt, YourInt>::value, "Types are the same");

    return 0;
}
```

### Scope of Template Aliases

> Alias templates must be declared at class scope or namespace scope

```cpp
namespace MyNamespace {
    template<typename T>
    using MyVector = std::vector<T>;
}

class MyClass {
public:
    template<typename T>
    using Ptr = T*;
};

// Error: cannot declare alias template in function scope
// void func() {
//     template<typename T>
//     using LocalAlias = T;  // Compilation error
// }
```

### Recursive Alias Restrictions

> Alias templates cannot directly or indirectly reference themselves

```cpp
template<typename T>
struct A;

// Error: recursive alias
// template<typename T>
// using B = typename A<T>::U;

template<typename T>
struct A {
    // typedef B<T> U;  // This will cause recursive definition error
};
```

## III. Exercise Code

### Exercise Code Topics

- 0 - [Basic Type Alias](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/14-type-alias-0.cpp)
- 1 - [Complex Types and Function Pointer Aliases](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/14-type-alias-1.cpp)
- 2 - [Alias Template Basics](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/14-type-alias-2.cpp)
- 3 - [Alias Template Applications in Standard Library](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/14-type-alias-3.cpp)

### Exercise Code Auto-Check Command

```bash
d2x checker type-alias
```

## IV. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
