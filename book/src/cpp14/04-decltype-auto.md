<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./04-decltype-auto.html
[English]: ../en/cpp14/04-decltype-auto.html

# decltype(auto)

C++14 引入 `decltype(auto)` 返回类型, 按 decltype 规则精确保留表达式类型, 解决 auto 返回剥离引用和 cv 限定的问题

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-auto](https://en.cppreference.com/w/cpp/language/auto) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/04-decltype-auto.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/04-decltype-auto-0.cpp) |  |


**为什么引入?**

- `auto` 返回会剥离引用和顶层 const, 在转发函数和 getter 中导致意外拷贝
- `decltype(auto)` 使用 decltype 规则推导, 精确保留返回表达式的值类别和类型修饰

**auto 返回 vs decltype(auto)**

```cpp
int x = 42;

auto         f1() { return (x); }  // 返回 int
decltype(auto) f2() { return (x); }  // 返回 int&, 保留引用
```

## 一、基础用法和场景

### 保留引用返回

```cpp
int global = 100;

decltype(auto) get_ref() {
    return (global);  // 返回 int&, 外部可修改
}

get_ref() = 200;  // 修改 global
```

### 转发模式

```cpp
template <typename T>
decltype(auto) forwarder(T&& arg) {
    return std::forward<T>(arg);  // 精确保留值类别
}
```

### 容器元素访问

```cpp
std::vector<int> v = {1, 2, 3};

decltype(auto) get_at(std::vector<int>& vec, size_t i) {
    return vec[i];  // 返回 int&, 允许外部修改
}

get_at(v, 0) = 10;  // v[0] 现在为 10
```

## 二、真实案例 - STL 中的 decltype(auto) 转发

> MSVC STL 使用 `decltype(auto)` 在转发调用时精确保留返回值类型。下面以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 为例 (源码: [`msvc-stl/stl/inc/xutility`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/xutility#L943-L947))

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (有删节)
template <class... _Args>
constexpr decltype(auto) operator()(_Args&&... _Vals) noexcept(...) {
    return _STD invoke(_Fn, _STD forward<_Args>(_Vals)...);
}
```

`std::invoke` 可能返回左值引用 (如 `std::get<T&>(tuple)`), 也可能返回右值 (如移动语义)。`decltype(auto)` 让返回值以原值类别呈现给调用方 — 不会因 auto 剥离而意外拷贝, 也不会丢失 const 限定

## 三、注意事项

### 括号陷阱

`decltype(auto)` 对 `return x;` 和 `return (x);` 的推导不同:

```cpp
int x = 10;
decltype(auto) f1() { return  x; }  // int
decltype(auto) f2() { return (x); } // int& — 注意!
```

### decltype(auto) 也可用于变量声明

```cpp
int x = 42;
decltype(auto) y = x;   // int
decltype(auto) z = (x);  // int& — 同样受括号影响
```

`decltype(auto)` 在变量声明和返回类型推导中都可用, 遵循同样的 decltype 推导规则

## 四、练习代码

### 练习代码主题

- 0 - [decltype(auto) — 引用保留与转发](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/04-decltype-auto-0.cpp)
- 1 - [decltype(auto) — 括号陷阱与变量声明](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/04-decltype-auto-1.cpp)

### 练习代码自动检测命令

```
d2x checker decltype-auto
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
