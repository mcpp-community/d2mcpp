<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./16-generalized-unions.md
[English]: ../en/cpp11/16-generalized-unions.md

# 广义联合体
在C++11之后引入了`generalized (non-trivial) unions`，广义非平凡联合体。

联合体的数据成员共享内存。
联合体的大小至少容纳最大的数据成员。

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-union](https://cppreference.com/w/cpp/language/union.html) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/16-generalized-unions.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/16-generalized-unions-0.cpp) |  |

**为什么引入**

- 可以直接放入诸如`std::string`的对象，不需要使用指针。
- 更好的管理数据成员的生命周期。

**现在的联合与之前有什么区别**

- 最多只有一个变体成员可以具有默认成员初始化器
- 联合体可以包含具有非平凡特殊成员函数的非静态数据成员。

```c++
union S {
  int a;
  float b;
  std::string str; // C++11 之前是不能直接放入这种数据成员的，或者使用静态成员
  S() {}
  ~S() {}
}
```

## 一、基础的用法和场景

**普通联合体的使用**
> 只有一个值是有效的
```cpp
union M {
  int a;
  double b;
  char *str;
}
```

**广义联合体的使用**
> 联合体的大小就是数据成员所占用的最大空间，其是随着数据成员的变化而动态变化的。
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
  ~M() { } // 需要知道哪个数据成员是有效的才能正确析构
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

**生命周期**
> 成员的生命周期从有效时开始，无效时结束。

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
  ~M() { } // 需要知道哪个数据成员是有效的才能析构
};

int main() {
  M m = 1;
  std::cout << "Life 1 time one Start" << std::endl;
  m = Life();
  // 该Life的生命周期会在失效前结束
  std::cout << "Life 1 time one End" << std::endl;
  m = 2;
  std::cout << "Life 2 time one Start" << std::endl;
  m = Life();
  std::cout << "Life 2 time one Start" << std::endl;
  m = 3;
  return 0;
}
```

**匿名联合体**

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

## 二、真实案例 - STL 中的广义联合体

**std::variant 的内部存储 - 递归广义联合体**
> 以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 中的 `std::variant` 实现为例 (源码: [`msvc-stl/stl/inc/variant`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/variant#L343-L399)), `_CONSTEXPR20` / `_STD` 是库内部宏, 阅读时可忽略

```cpp
// MSVC STL · msvc-stl/stl/inc/variant (有删节)
template <class _First, class... _Rest>
class _Variant_storage_<false, _First, _Rest...> {
public:
    union {
        remove_cv_t<_First> _Head;
        _Variant_storage<_Rest...> _Tail;
    };

    _CONSTEXPR20 ~_Variant_storage_() {
        // 联合体不知道哪个成员活跃, 析构由外层 variant 控制
    }
    // ...
};
```

`std::variant` 通过递归联合体在一块内存里容纳多个不同类型, 非平凡析构版本必须显式定义析构函数 — 这正是 C++11 广义联合体的核心能力: 联合体可以包含有非平凡特殊成员函数的成员, 但需要手动管理生命周期

**std::any 的小对象优化 - 联合体做类型擦除存储**
> `std::any` 使用联合体将小对象、大对象指针和原始缓冲区合并到同一块内存 (源码: [`msvc-stl/stl/inc/any`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/any#L362-L376))

```cpp
// MSVC STL · msvc-stl/stl/inc/any (有删节)
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

> 小结: `std::variant`、`std::any` 的核心存储都依赖广义联合体。C++11 之前联合体只能容纳 POD 类型, 标准库不得不用原始字节缓冲区 + placement new 的迂回方案; 广义联合体让代码可以直接表达"多选一"的内存布局, 并由外层封装负责跟踪活跃成员与管理生命周期, 因而更易维护

## 三、注意事项

**可访问性**

`union`和`struct`一样，默认的数据成员都是`public`。

**联合体的析构**
> 联合体的析构一般不指定，因为联合体本身是没办法得知自身的有效数据是哪个的。

```cpp
union M {
  char* str1;
  char* str2;
  ~M() {
    delete str1; // 如果有效数据是str2的话就会错误
  }
};
```

**匿名联合体的限制**
> 匿名联合体是无法包含成员函数和静态数据成员。

```cpp
union {
  int a;
  static int b; // 错误：不能有静态数据成员
  int print() {...}; // 错误：不能有成员函数
};
```

**未定义行为**
> 如果访问无效的数据成员，会发生不可预知的行为。

```cpp
union M {
  int a;
  double b;
};

M m;
m.a = 1;
double c = m.b; // 错误：未定义行为
```

## 四、练习代码

### 练习代码主题

- 0 - [联合体默认成员初始化 - 最多一个变体成员可带默认初始化器](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/16-generalized-unions-0.cpp)
- 1 - [联合体包含非平凡类型及生命周期管理 - placement new 构造 / 显式析构](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/16-generalized-unions-1.cpp)
- 2 - [带标签的鉴别联合体 - 用 enum + union 实现简易 variant](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/16-generalized-unions-2.cpp)

### 练习代码自动检测命令

```
d2x checker generalized-unions
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)