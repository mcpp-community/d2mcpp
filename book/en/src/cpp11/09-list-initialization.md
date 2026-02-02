<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/09-list-initialization.html
[English]: ./09-list-initialization.html

# List Initialization

List initialization is an initialization style that uses `{ arg1, arg2, ... }` lists (curly braces) to initialize objects, and can be used in almost all object initialization scenarios, hence it's often called **uniform initialization**. Additionally, it adds type checking for list members to prevent narrowing issues.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/list_initialization.html) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/09-list-initialization.md) | [Video Explanation](https://www.bilibili.com/video/BV1vKuQzkEo2) | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/09-list-initialization-0.cpp) |  |

**Why was it introduced?**

- Solve the problem of inconsistent initialization syntax styles
- Prevent narrowing issues caused by implicit conversions
- Facilitate container type initialization
- Resolve default initialization syntax pitfalls

## I. Basic Usage and Scenarios

### Uniform Initialization Style

Before C++11, different scenarios had different initialization methods:

```cpp
int a = 5;              // Copy initialization
int b(5);               // Direct initialization
int arr[3] = {1, 2, 3}; // Array initialization
Object obj1;            // Default construction
Object obj2(obj1);      // Copy construction
```

They can be unified in style using `{ }`:

```cpp
int a = { 5 };              // Copy initialization
int b { 5 };                // Direct initialization
int arr[3] = {1, 2, 3};     // Array initialization
Object obj1 { };            // Default initialization
Object obj2 { obj1 };       // Copy construction
```

### Avoid Implicit Type Conversion and Narrowing Issues

Traditional initialization methods generally follow the C language implicit type conversion rules. For example, when initializing an `int` type variable with a `double` type, the decimal part is automatically discarded. List initialization adds additional compile-time type checking to avoid implicit type conversions and precision loss issues. In modern C++, unless intentional implicit type conversion is needed, using list initialization is generally a better choice.

```cpp
int a = 3.3; // ok
int a = { 3.3 }; // error

constexpr double b { 3.3 }; // ok
int c(b); // ok -> 3
int c { b }; // error: type mismatch
```

Narrowing checks in array initialization:

```cpp
int arr[] { 1, 2, 3.3, 4 }; // error: 3.3 causes narrowing
int arr[] = { 1, 2, b, 4 }; // error: b causes narrowing
```

> Note: If b is a runtime variable, the compiler might only trigger narrowing warnings instead of errors.

### Improve Container Initialization Conciseness

For container type initialization, old C++ often required two steps. First, create an element array; second, use this array to initialize the container.

```cpp
int arr[5] = {1, 2, 3, 4, 5};
std::vector<int> v(arr, arr + sizeof(arr) / sizeof(int));
```

The introduction of list initialization allows us to combine these two steps into one, significantly improving container initialization conciseness.

```cpp
std::vector<int> v1 {1, 2, 3};
std::vector<int> v2 {1, 2, 3, 4, 3};
```

Moreover, through `std::initializer_list`, our custom types can also support this **variable-length** list initialization style.

```cpp
class MyVector {
public:
    MyVector() = default;
    MyVector(std::initializer_list<int> list) {
        for (auto it = list.begin(); it != list.end(); it++) {
            // *it ...
        }
    }
};
```

```cpp
MyVector v1 {1, 2, 3};
MyVector v2 {1, 2, 3, 4, 3};
```

### Avoid Initialization Syntax Pitfalls

Using `{ }` to call default constructors avoids syntax pitfalls.

```cpp
#include <iostream>

struct Object {
    Object() { std::cout << "Constructor called!" << std::endl; }
};

int main() {
    Object obj1 { };
    Object obj2(); // obj2 is a function, not an Object instance
}
```

## II. Important Notes

### Array Type List Initialization

The values in array type definitions are generally indeterminate, but list initialization performs default value initialization and supports automatic zero-padding.

Regular arrays:

```cpp
int arr[4];          // arr[0] indeterminate
int arr[4] { };      // arr[0] = 0
int arr[4] { 1, 2 }; // arr[2] / arr[3] automatically padded to 0
```

Array containers:

```cpp
std::array<int, 4> arr;     // arr[0] indeterminate/may be random value
std::array<int, 4> arr { }; // arr[0] == 0
std::array<int, 4> arr { 1, 2 }; // arr[0] == 1, arr[2] automatically padded to 0
```

### Member Initialization Issues

List initialization supports direct initialization of aggregate type members, but note that after adding constructors, they must match the constructor.

```cpp
struct Point {
    int x, y;
    // Point(int x, int y) { ... }
};
```

```cpp
Point { 1, 2 };
Point p1 { 2, 3 }; // p1 { x: 2, y: 3}
```

### Prefer `std::initializer_list` Constructors

```cpp
class MyVector {
public:
    MyVector() = default;
    MyVector(int x, int y) {  }
    MyVector(std::initializer_list<int> list) {
        for (auto it = list.begin(); it != list.end(); it++) {
            // *it ...
        }
    }
};
```

```cpp
MyVector v1 { 1, 2 }; // Prefers MyVector(std::initializer_list<int> list)
MyVector v1(1, 2); // Matches MyVector(int x, int y)
```

## III. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
