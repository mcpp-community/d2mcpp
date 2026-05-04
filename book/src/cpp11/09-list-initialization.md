<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./09-list-initialization.html
[English]: ../en/cpp11/09-list-initialization.html

# 列表初始化

列表初始是一种用`{ arg1, arg2, ... }`列表(大括号), 初始化对象的一种初始化风格, 并且可以用于几乎所有的对象初始化场景, 所以也常常称他为**统一初始化**。此外, 他还增加了列表成员的类型检查功能, 防止一些窄化问题

| Book | Video | Code | X | 
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/list_initialization.html) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/09-list-initialization.md) | [视频解读](https://www.bilibili.com/video/BV1vKuQzkEo2) | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/09-list-initialization-0.cpp) |  |

**为什么引入?**

- 解决初始化语法风格不统一问题
- 禁止隐式转换造成的窄化问题
- 方便容器类型的初始化
- 解决默认初始化语法陷阱

## 一、基础用法和场景

### 统一初始化风格

c++11之前不同场景有不同的初始化的方式

```cpp
int a = 5;              // 拷贝初始化
int b(5);               // 直接初始化
int arr[3] = {1, 2, 3}; // 数组初始化
Object obj1;            // 默认构造
Object obj2(obj1);      // 拷贝构造
```

他们可以用`{ }`进行风格统一

```cpp
int a = { 5 };              // 拷贝初始化
int b { 5 };                // 直接初始化
int arr[3] = {1, 2, 3};     // 数组初始化
Object obj1 { };            // 默认初始化
Object obj2 { obj1 };       // 拷贝构造
```

### 避免隐式类型转换和窄化问题

一般传统的初始化方法, 是默认C语言隐式类型转换规则风格. 例如, 用`double`类型初始化`int`类型变量的时候会自动丢掉小数位. 而列表初始化会增加额外的编译期类型检查来避免隐式类型转换和精度丢失问题. 在现代C++中, 除非有意的需要这种隐式类型转换, 大多数时候使用列表初始化是更好的选择

```cpp
int a = 3.3; // ok
int a = { 3.3 }; // error

constexpr double b { 3.3 }; // ok
int c(b); // ok -> 3
int c { b }; // error: 类型不匹配
```

数组初始化中的窄化检查

```cpp
int arr[] { 1, 2, 3.3, 4 }; // error: 3.3会发生窄化
int arr[] = { 1, 2, b, 4 }; // error: b会发生窄化
```

> 注: 如果b是运行时变量, 编译期可能只会触发窄化警告而不会报错

### 提高容器初始化的简洁性

对于容器类型的初始化, 老C++中常常会分成两个步骤。第一步, 创建一个元素数组; 第二步, 使用这个数组来初始化容器

```cpp
int arr[5] = {1, 2, 3, 4, 5};
std::vector<int> v(arr, arr + sizeof(arr) / sizeof(int));
```

而列表初始化的引入, 能让我们把两步合为一个步骤, 大幅度提高了容器初始化的简洁性

```cpp
std::vector<int> v1 {1, 2, 3};
std::vector<int> v2 {1, 2, 3, 4, 3};
```

并且, 可以通过`std::initializer_list`让我们的自定义类型也能支持这种**不定长**的列表初始化方式

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

### 避免初始化语法陷阱

使用`{ }`调用默认构造函数, 避免语法陷阱

```cpp
#include <iostream>

struct Object {
    Object() { std::cout << "Constructor called!" << std::endl; }
};

int main() {
    Object obj1 { };
    Object obj2(); // obj2是函数, 而不是Object对象
}
```

## 二、注意事项

### 数组类型列表初始化

数组类型的定义里面的值一般是不确定的, 但是列表初始化的方式会做默认值的初始化, 并支持自动补0

普通数组

```cpp
int arr[4];          // arr[0] 不确定
int arr[4] { };      // arr[0] = 0
int arr[4] { 1, 2 }; // arr[2] / arr[3] 会自动补成0
```

数组容器

```cpp
std::array<int, 4> arr;     // arr[0] 不确定/可能是随机值
std::array<int, 4> arr { }; // arr[0] == 0
std::array<int, 4> arr { 1, 2 }; // arr[0] == 1, arr[2] 会自动补成0
```

### 成员初始化问题

列表初始化支持直接对 聚合类型的成员做初始化, 但需要注意添加构造函数后必须要匹配构造函数才可以

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

### 优先匹配`std::initializer_list`的构造函数

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
MyVector v1 { 1, 2 }; // 会优先匹配 MyVector(std::initializer_list<int> list)
MyVector v1(1, 2); // 匹配MyVector(int x, int y)
```

## 三、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)