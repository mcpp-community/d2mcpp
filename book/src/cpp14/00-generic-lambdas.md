<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./00-generic-lambdas.html
[English]: ../en/cpp14/00-generic-lambdas.html

# 泛型 lambda - generic lambdas

泛型 lambda 是 C++14 在 lambda 表达式上引入的一项重要增强, 允许 lambda 的参数使用 `auto` 类型推导, 使得一个 lambda 表达式可以像函数模板一样处理不同类型的参数

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-lambda](https://en.cppreference.com/w/cpp/language/lambda) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/00-generic-lambdas.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/00-generic-lambdas-0.cpp) |  |


**为什么引入?**

- C++11 的 lambda 参数类型必须显式指定, 同一个 lambda 无法复用于不同类型的参数 — 本质上 lambda 的 `operator()` 是一个普通成员函数, 不能是模板
- 在实际编码中, 很多 lambda 的逻辑与类型无关 (例如 `[](int a, int b) { return a < b; }` 中的比较逻辑对 `int` / `double` / `string` 都适用), 但 C++11 要求为每种类型写一份
- C++14 允许 lambda 参数使用 `auto`, 编译器会为 `operator()` 生成一个隐式的模板, 每种参数类型实例化一份 — 相当于把"函数模板"带到了 lambda 世界里

**泛型 lambda 是如何实现的?**

编译器将泛型 lambda 展开为一个带有**模板化 `operator()`** 的仿函数类。例如 `[](auto a, auto b) { return a + b; }` 在内部等价于:

```cpp
struct __lambda {
    template <typename T1, typename T2>
    auto operator()(T1 a, T2 b) const {
        return a + b;
    }
};
```

## 一、基础用法和场景

### 简单泛型 lambda

> lambda 参数用 auto 声明, 编译器根据调用时的实参类型生成对应的 operator() 实例

```cpp
auto identity = [](auto x) {
    return x;
};

int    i = identity(42);       // x 推导为 int
double d = identity(3.14);     // x 推导为 double
```

### 多参数泛型 lambda

```cpp
auto add = [](auto a, auto b) {
    return a + b;
};

add(1, 2);           // int + int
add(1.5, 2.5);       // double + double
add(std::string("hello "), std::string("world")); // string + string
```

每个参数的类型是独立推导的, `T1` 和 `T2` 可以不同:

```cpp
auto multiply = [](auto a, auto b) {
    return a * b;
};

multiply(2, 3.5);    // int * double → double
```

### 泛型 lambda + STL 算法

泛型 lambda 最常见的应用场景是配合 STL 算法, 避免为每种容器元素类型重复编写相同逻辑的比较/判据:

```cpp
std::vector<int> v1 = {5, 1, 4, 2, 8};
std::vector<double> v2 = {3.1, 2.7, 8.5, 1.9};

// C++11: 需要为 int 和 double 分别写 lambda
std::sort(v1.begin(), v1.end(), [](int a, int b) { return a > b; });
std::sort(v2.begin(), v2.end(), [](double a, double b) { return a > b; });

// C++14: 同一个泛型 lambda 搞定
auto gt = [](auto a, auto b) { return a > b; };
std::sort(v1.begin(), v1.end(), gt);
std::sort(v2.begin(), v2.end(), gt);
```

### 带捕获的泛型 lambda

捕获的变量类型不变, 只有参数用 `auto`:

```cpp
int threshold = 10;
auto above = [threshold](auto x) {
    return x > threshold;  // threshold 捕获为 int, x 是泛型参数
};

above(20);   // x = int
above(3.5);  // x = double
```

### 泛型 lambda 返回 lambda

泛型 lambda 可以返回一个新 lambda, 实现类似"函数工厂"的效果:

```cpp
auto make_adder = [](auto n) {
    return [n](auto x) { return x + n; };  // C++14 支持, 返回类型自动推导
};

auto add5 = make_adder(5);
add5(10);   // 15
add5(3.14); // 8.14
```

## 二、真实案例 - STL 中的透明比较器与泛型 lambda

> C++14 同步引入了**透明运算符仿函数** (transparent operator functors, 如 `std::less<>` / `std::greater<>`), 它们和泛型 lambda 是同一动机的两个出口: 都消除了"必须写死类型"的限制。下面以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 为例 (源码: [`msvc-stl/stl/inc/xutility`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/xutility#L902-L912)), `_NODISCARD` / `constexpr` 是库内部标注, 阅读时可忽略

### std::greater 的透明特化 — 让比较器"看懂"任意类型

C++11 的 `std::greater<T>` 绑死了模板参数 `T`, 比如 `std::greater<int>` 只能比较 `int`。C++14 新增了 `std::greater<>` (等价于 `std::greater<void>`), 它的 `operator()` 本身是一个模板, 可以接受任意可比较的类型:

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (有删节)
template <>
struct greater<void> {
    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(...) -> decltype(static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right)) {
        return static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};
```

`operator()` 是一个模板成员函数 — 这和泛型 lambda 的底层实现完全一致。`is_transparent` 标签向 `std::set` / `std::map` 等关联容器声明此比较器支持异构查找, 可以直接用 `std::string` 查找 `std::set<std::string>` 而不需要构造临时对象

### 透明比较器 + 泛型 lambda — 同一场景的两种写法

泛型 lambda 和透明比较器可以互换:

```cpp
std::vector<int> v = {5, 1, 4, 2, 8};

// 方式一: C++14 透明比较器
std::sort(v.begin(), v.end(), std::greater<>());

// 方式二: C++14 泛型 lambda
std::sort(v.begin(), v.end(), [](auto a, auto b) { return a > b; });
```

两者都能避免 C++11 中为 `int` / `double` / `string` 分别写比较器的冗余

> 小结: C++14 的透明比较器和泛型 lambda 都是 "参数类型参数化" 思路的产物 — 一个作用在仿函数上, 一个作用在 lambda 上。理解了两者共用 `is_transparent` 标签和 STL 算法的配合方式, 就能明白标准库为什么要同时引入这两个特性

## 三、注意事项

### 泛型 lambda 是一个有模板 operator() 的类

每个泛型 lambda 表达式产生一个独立的闭包类型。即使是写法完全相同的两个泛型 lambda, 它们的类型也不同 — 这和普通 lambda 的规则一样, 但泛型 lambda 的 `operator()` 是模板, 所以同一类型可以接受不同参数类型

```cpp
auto f = [](auto x) { return x; };
auto g = [](auto x) { return x; };
// f 和 g 的类型不同, 不能互相赋值
```

### 泛型参数和完美转发

泛型 lambda 的参数推导默认剥离引用和 const, 需要完整保留时用 `auto&&` 配合 `std::forward`:

```cpp
auto forwarder = [](auto&& x) -> decltype(auto) {
    return std::forward<decltype(x)>(x);
};
```

这种写法在泛型 lambda 里很常见, 也是 `decltype(auto)` (C++14 的另一特性) 的典型使用场景

### 泛型 lambda 可以是 variadic

C++14 的泛型 lambda 支持参数包 — `[](auto... xs)` 可以接受任意数量(和类型)的参数, 编译器为 `operator()` 生成带参数包的模板。C++20 进一步允许显式模板参数列表 (`[]<typename... Ts>(Ts... xs)`), 但变参泛型 lambda 在 C++14 就已经可用

## 四、练习代码

### 练习代码主题

- 0 - [泛型 lambda 基础 — auto 参数与类型推导](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/00-generic-lambdas-0.cpp)
- 1 - [泛型 lambda 与 STL 算法 — 排序、查找、函数工厂](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/00-generic-lambdas-1.cpp)

### 练习代码自动检测命令

<details>
<summary>还没有 d2x?点击展开获取方式</summary>

```bash
# 1. 安装 xlings(Linux / macOS)
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash
# Windows PowerShell:
# irm https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.ps1 | iex

# 2. 安装 d2x 并拉取本教程
xlings install d2x -y
d2x install d2mcpp

# 3. 进入项目目录 & 运行检查命令
cd d2mcpp
```

</details>

```
d2x checker generic-lambdas
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
