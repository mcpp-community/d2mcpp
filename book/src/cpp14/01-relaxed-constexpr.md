<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./01-relaxed-constexpr.html
[English]: ../en/cpp14/01-relaxed-constexpr.html

# 放宽的 constexpr - relaxed constexpr

C++14 大幅放宽了 `constexpr` 函数的限制——允许在编译期函数中使用循环、分支、局部变量和多条语句, 让更多的算法可以在编译期执行

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-constexpr](https://en.cppreference.com/w/cpp/language/constexpr) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/01-relaxed-constexpr.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/01-relaxed-constexpr-0.cpp) |  |


**为什么引入?**

- C++11 的 `constexpr` 函数体必须是 `return expr;` 一条语句, 任何循环或分支都要用递归 + 三元运算符表达, 代码晦涩
- 实际算法中循环和分支是最基本的控制流, 将它们排除在 constexpr 之外严重限制了编译期计算的应用范围
- C++14 允许 constexpr 函数使用 `for` / `while` / `if` / `switch` 和局部变量, 让"把运行期算法搬到编译期"成为可能, 而不需要改写风格

**C++11 vs C++14 constexpr**

```cpp
// C++11: 只能用递归 + 三元运算符
constexpr int factorial_11(int n) {
    return n <= 1 ? 1 : n * factorial_11(n - 1);
}

// C++14: 可以写正常的循环
constexpr int factorial_14(int n) {
    int result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}
```

## 一、基础用法和场景

### 循环结构 — for / while

> constexpr 函数中可以像普通函数一样写循环

```cpp
constexpr int sum_to(int n) {
    int total = 0;
    for (int i = 1; i <= n; ++i) {
        total += i;
    }
    return total;
}

static_assert(sum_to(5) == 15, "");
static_assert(sum_to(100) == 5050, "");
```

### 分支结构 — if / switch

```cpp
constexpr int abs_val(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

static_assert(abs_val(-42) == 42, "");
static_assert(abs_val(0) == 0, "");

constexpr int day_count(int month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return 28;
        default:
            return 0;
    }
}

static_assert(day_count(7) == 31, "");
```

### 局部变量与多条语句

```cpp
constexpr double circle_area(double radius) {
    const double pi = 3.14159;
    double r2 = radius * radius;
    return pi * r2;
}

static_assert(circle_area(1.0) == 3.14159, "");
```

### 编译期实用算法 — 循环斐波那契

> 用循环替代递归, 在编译期计算斐波那契数列

```cpp
constexpr int fib(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

static_assert(fib(10) == 55, "");
static_assert(fib(0) == 0, "");
```

## 二、真实案例 - STL 中的 constexpr 编译期索引序列

> C++14 同步引入了 `std::integer_sequence` 和 `std::make_index_sequence`, 它们正是依赖放宽后的 constexpr 才能实现的编译期工具。下面以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 为例 (源码: [`msvc-stl/stl/inc/tuple`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/tuple#L323-L343)), `_Tag` / `_Tpl` 是库内部标记和类型, 阅读时可忽略

### std::tuple 的编译期构造 — index_sequence 展开参数包

`std::tuple` 需要从一个"类元组"对象中逐元素提取并构造自己的成员, 这个过程必须在编译期完成。MSVC STL 使用 `make_index_sequence` 生成编译期索引, 配合参数包展开实现:

```cpp
// MSVC STL · msvc-stl/stl/inc/tuple (有删节)
template <size_t... _Indices, class _Tpl>
constexpr tuple(_Tag, _Tpl&& _Right, index_sequence<_Indices...>)
    : _Mybase(static_cast<_Tpl&&>(_Right)._Get_rest()) {
    // _Indices... 是编译期生成的 0, 1, 2, ... N-1 索引序列
    // 通过 get<_Indices>(_Right) 逐元素提取并构造
}

// 公开构造函数 — 用 make_index_sequence 自动生成索引
explicit(false) constexpr tuple(_Exact_args_t, _Tpl&& _Right)
    : tuple(_Tag{}, _STD forward<_Tpl>(_Right),
        make_index_sequence<tuple_size_v<remove_reference_t<_Tpl>>>{}) {}
```

`make_index_sequence<N>` 在编译期生成 `index_sequence<0, 1, ..., N-1>`, 让 tuple 构造可以用 `get<0>` / `get<1>` / ... 依次提取元素。这正是 C++14 放宽 constexpr 后最经典的应用 — 循环不再是运行期概念, 而是通过编译期整数序列 + 参数包展开来完成

### std::integer_sequence — C++14 引入的编译期整数载体

```cpp
// MSVC STL · msvc-stl/stl/inc/utility (有删节)
template <class _Ty, _Ty... _Vals>
struct integer_sequence {
    static_assert(is_integral_v<_Ty>,
        "integer_sequence<T, I...> requires T to be an integral type.");
};

template <size_t... _Vals>
using index_sequence = integer_sequence<size_t, _Vals...>;

template <size_t _Size>
using make_index_sequence = __make_integer_seq<integer_sequence, size_t, _Size>;
```

> 小结: `std::make_index_sequence` 和 `std::tuple` 的编译期构造都依赖 C++14 放宽后的 constexpr 环境。没有 loop + branch 的 constexpr, 标准库就只能在编译器内部用黑魔法生成整数序列, 而不能用 C++ 语言自身的表达能力来实现

## 三、注意事项

### C++14 constexpr 仍不支持的操作

以下操作在 C++14 constexpr 函数中仍被禁止:

- `goto` 语句
- `try` / `catch` 异常处理
- `static` 或 `thread_local` 局部变量
- 内联汇编
- 未初始化的局部变量

### constexpr 函数的"两面性"

constexpr 函数可以在编译期执行, 也可以在运行期执行——取决于调用上下文:

```cpp
constexpr int fib(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

static_assert(fib(10) == 55, "");  // 编译期执行

int main() {
    int n = std::rand() % 20;
    return fib(n);                  // 运行期执行, 同一份代码
}
```

### constexpr 不是 inline 的替代

把函数标记为 constexpr 并不改变其 ODR 链接属性 (C++17 起 constexpr 函数才隐式 inline), 也不代表所有能 constexpr 的都该 constexpr。如果一个函数几乎只在运行期调用, 加 constexpr 只增加了接口约束, 实际收益很小

## 四、练习代码

### 练习代码主题

- 0 - [constexpr 循环 — 编译期阶乘和幂运算](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/01-relaxed-constexpr-0.cpp)
- 1 - [constexpr 分支和局部变量 — 编译期条件判断与斐波那契](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/01-relaxed-constexpr-1.cpp)

### 练习代码自动检测命令

```
d2x checker relaxed-constexpr
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
