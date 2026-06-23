<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./08-aggregate-nsdmi.html
[English]: ../en/cpp14/08-aggregate-nsdmi.html

# 聚合初始化与 NSDMI

C++14 放宽了聚合类型的限制: 拥有非静态数据成员初始化器 (NSDMI) 的类仍然可以是聚合, 可以使用 `{}` 初始化

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-aggregate_initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/08-aggregate-nsdmi.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/08-aggregate-nsdmi-0.cpp) |  |


**为什么引入?**

C++11 中, 只要类有 NSDMI (`int x = 0;`), 它就不再是聚合, `{}` 初始化失效。这导致给简单数据结构加默认值后反而不能用简洁的聚合初始化。C++14 修复了这个矛盾

## 一、基础用法和场景

```cpp
struct Point {
    int x = 0;  // NSDMI
    int y = 0;
};

Point p1{3, 4};   // C++14: OK, 聚合初始化
Point p2{1};      // y 使用默认值 0
Point p3{};       // x=0, y=0
```

## 二、真实案例 - STL 中的聚合 + NSDMI

> MSVC STL 内部大量使用带 NSDMI 的聚合类型。下面以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 为例 (源码: [`msvc-stl/stl/inc/array`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/array#L20-L23))

```cpp
// MSVC STL · msvc-stl/stl/inc/array (有删节)
struct _Iterator_base12_compatible {
    _Container_proxy* _Myproxy{};
    _Iterator_base12* _Mynextiter{};
};
```

两个成员都有 `{}` 默认初始化, 但该类型仍以聚合形式参与迭代器基类的多重继承和初始化, 依赖的正是 C++14 对聚合 NSDMI 的放宽

## 三、注意事项

- NSDMI 的值只在没有提供对应初始化器时使用
- `{}` 初始化的值会覆盖 NSDMI 默认值
- 聚合的成员必须全部为 public

## 四、练习代码

### 练习代码主题

- 0 - [聚合 NSDMI — 带默认成员的类聚合初始化](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/08-aggregate-nsdmi-0.cpp)

### 练习代码自动检测命令

```
d2x checker aggregate-nsdmi
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
