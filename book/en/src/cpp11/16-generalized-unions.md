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
| [cppreference-union](https://cppreference.com/w/cpp/language/union.html) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/16-generalized-unions.md) | [Video Explanation]() | [Exercise Code]() |  |

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

## II. Precautions

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

## III. Exercise Code

TODO

## IV. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)