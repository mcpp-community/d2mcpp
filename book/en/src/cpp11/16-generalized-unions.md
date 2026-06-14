<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./16-generalized-unions.md
[English]: ../en/cpp11/16-generalized-unions.md

# Generalized Unions

C++11 introduced `generalized (non-trivial) unions`.

Union members share memory.
The size of a union is at least large enough to hold the largest data member.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-union](https://cppreference.com/w/cpp/language/union.html) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/16-generalized-unions.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/16-generalized-unions-0.cpp) |  |

**Why introduced?**

- Can directly hold objects like std::string, without needing pointers.
- Better management of member lifetimes.

**How Current Unions Differ from Before?**

- At most one variant member can have a default member initializer.
- Unions can contain non-static data members with non-trivial special member functions.

```c++
union S {
  int a;
  float b;
  std::string str; // Before C++11, such members could not be placed directly, or static members were used.
  S() {}
  ~S() {}
}
```

## I. Basic Usage and Scenarios

**Usage of Ordinary Unions**
> Only one value is valid at a time.
```cpp
union M {
  int a;
  double b;
  char *str;
}
```

**Usage of Generalized Unions**
> The size of the union is the maximum space occupied by its data members, which changes dynamically based on the active member.
```cpp
#include <iostream>
#include <string>
#include <vector>

union M {
  int a;
  int b;
  std::string str;
  std::vector<int> arr;
  M(int a) : b(a) { }
  M(const std::string &s) : str(s) { }
  M(const std::vector<int> &a) : arr(a) { }
  ~M() { } // Needs to know which data member is active to destruct correctly.
};

int main() {
  M m("123456");
  std::cout << "m.str = " << m.str<< std::endl;
  m.arr = { 1, 2, 3, 4, 5, 6 };
  std::cout << "m.arr = ";
  for(int v: m.arr) {
    std::cout << v << " ";
  }
  std::cout << std::endl;
  return 0;
}
```

**Lifetime**
> A member's lifetime begins when it becomes active and ends when it becomes inactive.

```cpp
#include <iostream>

struct Life {
  Life() { std::cout << "----Life(" << this << ") Start----" << std::endl; }
  ~Life() { std::cout << "----Life(" << this << ") End----" << std::endl; }
};

union M {
  int a;
  Life l;
  M(int n) : a(n) { }
  M(const Life &life) : l(life) { }
  ~M() { } // Needs to know which data member is active to destruct.
};

int main() {
  M m = 1;
  std::cout << "Life 1 time one Start" << std::endl;
  m = Life();
  // The Life object's lifetime ends before it becomes inactive here.
  std::cout << "Life 1 time one End" << std::endl;
  m = 2;
  std::cout << "Life 2 time one Start" << std::endl;
  m = Life();
  std::cout << "Life 2 time one Start" << std::endl;
  m = 3;
  return 0;
}
```

**Anonymous Unions**

```cpp
int main() {
  union {
    int a;
    const char *b;
  };
  a = 1;
  b = "Jerry";
}
```

## II. Real-World Case — Generalized Unions in the STL

**std::variant Internal Storage — Recursive Generalized Union**
> Using the `std::variant` implementation from the vendored [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) as an example (source: [`msvc-stl/stl/inc/variant`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/variant#L343-L399)), `_CONSTEXPR20` / `_STD` are internal macros and can be ignored while reading

```cpp
// MSVC STL · msvc-stl/stl/inc/variant (abridged)
template <class _First, class... _Rest>
class _Variant_storage_<false, _First, _Rest...> {
public:
    union {
        remove_cv_t<_First> _Head;
        _Variant_storage<_Rest...> _Tail;
    };

    _CONSTEXPR20 ~_Variant_storage_() {
        // The union does not know which member is active; destruction is
        // controlled by the outer variant class
    }
    // ...
};
```

`std::variant` uses a recursive union to hold multiple types in a single block of memory. The non-trivial destructor variant must explicitly define the destructor — this is exactly the key capability of C++11 generalized unions: unions can contain members with non-trivial special member functions, but their lifecycle must be managed manually

**std::any Small-Object Optimization — Union as Type-Erased Storage**
> `std::any` uses a union to combine small-object storage, heap pointers, and raw byte buffers into the same memory (source: [`msvc-stl/stl/inc/any`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/any#L362-L376))

```cpp
// MSVC STL · msvc-stl/stl/inc/any (abridged)
class any {
    struct _Storage_t {
        union {
            unsigned char _TrivialData[_Any_trivial_space_size];
            _Small_storage_t _SmallStorage;
            _Big_storage_t _BigStorage;
        };
        uintptr_t _TypeData;
    };

    union {
        _Storage_t _Storage{};
        max_align_t _Dummy;
    };
};
```

> Summary: The core storage of both `std::variant` and `std::any` relies on generalized unions. Before C++11, unions could only hold POD types, forcing the standard library to use raw byte buffers + placement new as a workaround. Generalized unions allow code to directly express "one-of-many" memory layout, with an outer wrapper responsible for tracking the active member and managing its lifecycle, making the code easier to maintain

## III. Precautions

**Accessibility**

Like `struct`, the default member access for a `union` is `public`.

**Destruction of Unions**
> Destructors for unions are generally not defined because the union itself cannot know which member is active.

```cpp
union M {
  char* str1;
  char* str2;
  ~M() {
    delete str1; // Error if the active member is str2.
  }
};
```

**Limitations of Anonymous Unions**
> Anonymous unions cannot contain member functions or static data members.

```cpp
union {
  int a;
  static int b; // Error: cannot have static data members.
  int print() {...}; // Error: cannot have member functions.
};
```

**Undefined Behavior**
> Accessing an inactive member results in undefined behavior.

```cpp
union M {
  int a;
  double b;
};

M m;
m.a = 1;
double c = m.b; // Error: undefined behavior.
```

## IV. Exercise Code

### Exercise Code Topics

- 0 - [Union Default Member Initialization](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/16-generalized-unions-0.cpp)
- 1 - [Union with Non-Trivial Types and Lifecycle Management](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/16-generalized-unions-1.cpp)
- 2 - [Tagged Discriminated Union — A Simplified std::variant with enum + union](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/16-generalized-unions-2.cpp)

### Exercise Auto-Checker Command

```
d2x checker generalized-unions
```

## V. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)