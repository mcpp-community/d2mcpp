<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/12-nullptr.html
[English]: ./12-nullptr.html

# nullptr - Pointer Literal

`nullptr` is a **pointer literal** introduced in C++11, used to represent null pointers. It addresses the shortcomings of traditional null pointer representations (such as `NULL` and `0`) in terms of type safety and overload resolution.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/nullptr) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/12-nullptr.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/12-nullptr-0.cpp) |  |

**Why was it introduced?**

- Resolve ambiguity issues with `NULL` macro and integer `0` in overload resolution
- Provide type-safe null pointer representation
- Clearly distinguish between pointer and integer types
- Support type deduction in template programming

**What's the difference between nullptr and NULL?**

- `nullptr` is a keyword introduced in C++11, with type `std::nullptr_t`
- `NULL` is a preprocessor macro, typically defined as integer `0` or `(void*)0`
- `nullptr` is more precise in overload resolution and won't be confused with integer types

## I. Basic Usage and Scenarios

### Replacing NULL and 0

> Used for pointer variable initialization and assignment, replacing traditional `NULL` and `0`

```cpp
int* ptr1 = nullptr;        // Recommended usage
int* ptr2 = NULL;           // Traditional usage
int* ptr3 = 0;              // Not recommended

// Check if pointer is null
if (ptr1 == nullptr) {
    // Handle null pointer case
}
```

### Resolving Overload Ambiguity

> Explicitly passing null pointers in function calls, `nullptr` can avoid overload ambiguity issues and prevent confusion with integer types

```cpp
void func(int* ptr) {
    if (ptr != nullptr) {
        *ptr = 42;
    }
}

void func(int value) {
    // Handle integer parameter
}

int main() {
    func(nullptr);  // Explicitly call pointer version
    func(0);        // May call integer version, causing ambiguity
    func(NULL);     // May call integer version, causing ambiguity
}
```

For example, in the code above, calling `func(NULL)` will report an overload ambiguity error

```bash
main.cpp: In function 'int main()':
main.cpp:16:9: error: call of overloaded 'func(NULL)' is ambiguous
   16 |     func(NULL);     // May call integer version, causing ambiguity
      |     ~~~~^~~~~~
```

### Ensuring Type Safety in Template Programming

> In template functions and classes, `nullptr` provides better type deduction and safety

```cpp
// https://en.cppreference.com/w/cpp/language/nullptr.html

template<class T>
constexpr T clone(const T& t) {
    return t;
}

void g(int*) {
    std::cout << "Function g called\n";
}

int main() {
    g(nullptr);        // ok
    g(NULL);           // ok
    g(0);              // ok

    g(clone(nullptr)); // ok
    g(clone(NULL));    // ERROR: NULL might be deduced to non-"pointer" type
    g(clone(0));       // ERROR: 0 will be deduced to non-"pointer" type
}
```

When using function templates, `NULL` and `0` are usually deduced to non-"pointer" types, while `nullptr` can avoid this problem

```bash
main.cpp:19:12: error: invalid conversion from 'int' to 'int*' [-fpermissive]
   19 |     g(clone(0));       // ERROR: 0 will be deduced to non-"pointer" type
      |       ~~~~~^~~
      |            |
      |            int
```

### Smart Pointers and Containers

> Used with modern C++ features (such as smart pointers, STL containers)

```cpp
#include <memory>
#include <vector>

int main() {
    std::shared_ptr<int> sp1 = nullptr;
    std::unique_ptr<int> up1 = nullptr;

    std::vector<int*> vec;
    vec.push_back(nullptr);

    // Check if smart pointer is null
    if (sp1 == nullptr) {
        sp1 = std::make_shared<int>(42);
    }
}
```

## II. Important Notes

### Type Deduction and std::nullptr_t

The type of `nullptr` is `std::nullptr_t`, which is a special type that can be **implicitly** converted to any pointer type:

```cpp
#include <cstddef>  // Contains definition of std::nullptr_t

void func(int*) {}
void func(double*) {}
void func(std::nullptr_t) {}

int main() {
    auto ptr = nullptr;  // ptr's type is std::nullptr_t

    func(nullptr);       // Call std::nullptr_t version
    func(ptr);           // Call std::nullptr_t version

    int* intPtr = nullptr;
    func(intPtr);        // Call int* version
}
```

### Implicit Conversion to Boolean Type

`nullptr` can be implicitly converted to `bool` type, which is very convenient in conditional checks:

```cpp
int* ptr = nullptr;

if (ptr) { // Equivalent to if (ptr != nullptr)
    // Pointer is not null
} else {
    // Pointer is null
}

bool isEmpty = (ptr == nullptr);  // true
```

## III. Practice Code

### Practice Code Topics

- 0 - [nullptr Basic Usage](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/12-nullptr-0.cpp)
- 1 - [nullptr Function Overloading](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/12-nullptr-1.cpp)
- 2 - [nullptr Advantages in Template Programming](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/12-nullptr-2.cpp)

### Auto-Checker Command

```bash
d2x checker nullptr
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://xlings.d2learn.org)
