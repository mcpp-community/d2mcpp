<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./02-variable-templates.html
[English]: ../en/cpp14/02-variable-templates.html

# 变量模板 - variable templates

变量模板是 C++14 引入的语法扩展, 允许变量本身成为模板, 在特化时根据类型参数产生不同的值或对象

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-variable_template](https://en.cppreference.com/w/cpp/language/variable_template) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/02-variable-templates.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/02-variable-templates-0.cpp) |  |


**为什么引入?**

- C++11 的模板只能套在函数和类上, 数值常量只能写成 static constexpr 成员或枚举
- 类型萃取大量使用 some_trait<T>::value, 每次都要写 ::value 后缀
- C++14 变量模板让 pi<double> / is_same_v<T, U> 这种写法成为可能

## 一、基础用法和场景

### 基本变量模板

```cpp
template <typename T>
constexpr T pi = T(3.1415926535897932385);

double d = pi<double>;   // 3.1415926535897932385
float  f = pi<float>;    // 3.14159265f
int    i = pi<int>;      // 3
```

### 类型萃取 _v 后缀

```cpp
template <typename T, typename U>
constexpr bool is_same_v = std::is_same<T, U>::value;

static_assert(is_same_v<int, int>,   "");
static_assert(!is_same_v<int, float>, "");
```

### 编译期配置值

```cpp
template <typename T>
constexpr size_t max_buffer_size = 1024;

template <>
constexpr size_t max_buffer_size<double> = 4096;
```

### 递归变量模板

```cpp
template <int N>
constexpr int factorial_v = N * factorial_v<N - 1>;

template <>
constexpr int factorial_v<0> = 1;

static_assert(factorial_v<5> == 120, "");
```

## 二、真实案例 - STL 中的变量模板

> MSVC STL 内部大量使用变量模板替代旧的 `::value` 萃取类。下面以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 为例 (源码: [`msvc-stl/stl/inc/xutility`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/xutility#L1458-L1462))

### _Is_iterator_v — 变量模板简化类型萃取

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (有删节)
template <class _Ty, class = void>
constexpr bool _Is_iterator_v = false;

template <class _Ty>
constexpr bool _Is_iterator_v<_Ty, void_t<_Iter_cat_t<_Ty>>> = true;

// 外部通过 _Is_iterator_v<_Ty> 直接使用, 无需 ::value
template <class _Ty>
struct _Is_iterator : bool_constant<_Is_iterator_v<_Ty>> {};
```

`_Is_iterator_v` 是变量模板 — 默认 false, 当 `_Ty` 拥有 `_Iter_cat_t` 时偏特化为 true, 替代了传统的 `_Is_iterator<T>::value` 写法。C++14 引入的变量模板语言机制, 让 STL 可以用 `_v` 后缀变量替代 `::value` 萃取类, 为 C++17 标准化 `is_same_v` 等 `_v` 萃取铺平了道路

## 三、注意事项

变量模板支持全特化和偏特化（这与函数模板不同, 后者只能全特化）。不要滥用 — 有些值用 constexpr 就够了。

## 四、练习代码

### 练习代码主题

- 0 - [变量模板基础 — pi 常量和编译期配置值](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/02-variable-templates-0.cpp)
- 1 - [类型萃取 _v 后缀 — 实现 is_void_v 和 is_same_v](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/02-variable-templates-1.cpp)

### 练习代码自动检测命令

```
d2x checker variable-templates
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
