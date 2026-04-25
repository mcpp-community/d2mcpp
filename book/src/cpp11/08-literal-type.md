<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./08-literal-type.html
[English]: ../en/cpp11/08-literal-type.html

# 字面值类型 - literal type

字面值类型 (LiteralType) 是 C++11 引入的一类 "可以参与编译期计算" 的类型. 它和 ch07 的 constexpr 是一对搭档 - constexpr 决定 "函数 / 变量能否在编译期被求值", 而字面值类型决定 "什么样的值能进入这套编译期世界". 同时, C++11 还顺势放开了 **用户自定义字面值** (`42_km`, `"abc"_s`), 让自定义类型也能写出像内置字面量一样的语法

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-LiteralType](https://en.cppreference.com/w/cpp/named_req/LiteralType) / [cppreference-user-literal](https://en.cppreference.com/w/cpp/language/user_literal) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/08-literal-type.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/08-literal-type-0.cpp) |  |


**为什么引入?**

- 配合 constexpr 把 "用户自定义类型" 也能参与编译期计算, 而不仅限于 int / double 这些内置类型
- 给单位 / 度量 / 强类型整数提供更可读的字面值写法 (例如 `1_km`, `3_sec`, `42ms`)
- 让 "在编译期就能算出结果" 的代码范围从内置标量扩展到 Vector / Point / Color 这类领域类型

**什么样的类型能算字面值类型?**

- 内置标量类型 (int / double / pointer / nullptr_t / 枚举 等) 自动满足
- 数组: 元素是字面值类型的数组本身也是字面值类型
- 用户类型: 至少一个 constexpr 构造函数 (而且不是拷贝 / 移动构造) + 平凡或 constexpr 的析构 + 所有非静态数据成员都是字面值类型 + 所有基类都是字面值类型

## 一、基础用法和场景

### 内置类型的字面值

内置标量类型天然就是字面值类型, 直接配合 constexpr 即可在编译期参与计算

```cpp
constexpr char c = 'A';
constexpr int  a = 1;
constexpr double pi = 3.14;

constexpr int sum = a + 2 + 3;  // 编译期就算好
```

### 把用户类型变成字面值类型 (添加 constexpr 构造)

默认写一个普通的 `Vector` 类, 它**不是**字面值类型, 也就没法在 constexpr 上下文里使用

```cpp
struct Vector {
    int x, y;
    Vector(int x_, int y_) : x(x_), y(y_) { } // 普通构造, 非 constexpr
};

constexpr Vector v{1, 2}; // 错误: Vector 没有 constexpr 构造函数
```

只要把构造函数标记为 `constexpr`, 就把 `Vector` 升级成了字面值类型, 可以放进 constexpr 函数里做编译期组合

```cpp
struct Vector {
    int x, y;
    constexpr Vector(int x_, int y_) : x(x_), y(y_) { } // 关键: constexpr 构造
};

constexpr Vector add(const Vector& a, const Vector& b) {
    return Vector(a.x + b.x, a.y + b.y);
}

constexpr Vector v1{1, 2}, v2{2, 3};
constexpr Vector v3 = add(v1, v2); // 编译期就算出 {3, 5}
```

### 字面值类型 + constexpr 函数 = 编译期就能算的小型计算

把字面值类型和 constexpr 函数结合起来, 可以把过去只能写在运行时的 "小型业务逻辑" 整体迁移到编译期. 例如下面把字符串拆成数组 / 把数组求和都在编译期完成

```cpp
constexpr std::array<int, 3> to_array(const char *str) {
    return { str[0] - '0', str[1] - '0', str[2] - '0' };
}

constexpr auto arr = to_array("123");
constexpr int sum  = arr[0] + arr[1] + arr[2]; // 编译期就是 6
```

### 用户自定义字面值 - operator"" _suffix

C++11 允许定义形如 `42_km` / `"abc"_s` 的字面量, 写法是重载 `operator"" _suffix`. 后缀名**必须以下划线开头** - 不带下划线的后缀是标准库保留的

```cpp
struct Length {
    long double meters;
};

// 浮点字面量后缀: 1.5_km
constexpr Length operator"" _km(long double v) {
    return Length{ v * 1000.0L };
}

// 整型字面量后缀: 200_m
constexpr Length operator"" _m(unsigned long long v) {
    return Length{ static_cast<long double>(v) };
}

constexpr Length d1 = 1.5_km; // 1500 m
constexpr Length d2 = 200_m;  // 200 m
```

字符串字面量也能定义后缀, 例如自定义一个 `_s` 转 `std::string`

```cpp
std::string operator"" _s(const char* str, std::size_t len) {
    return std::string(str, len);
}

auto greet = "hello"_s; // std::string
```

## 二、注意事项

### 自定义字面值后缀必须以下划线开头

不带下划线的后缀 (例如 `s`, `min`, `if`) 是标准库保留的. 用户自定义字面值如果不加下划线, 行为是未定义的, 编译器一般也会直接给警告

```cpp
// 错误示范: 没有下划线
long double operator"" km(long double v); // 警告 / 未定义

// 正确写法
long double operator"" _km(long double v); // ok
```

### "字面值类型" 不等于 "编译期常量"

字面值类型说的是 **类型本身有没有资格参与 constexpr**, 不代表这个类型的每个值都是编译期已知的. 一个普通的 `int` 变量也是字面值类型, 但它的值完全可能是运行时输入

```cpp
int x;
std::cin >> x;        // x 的值是运行时确定的
// 但 int 仍然是字面值类型 - 类型资格 != 值已知
```

要让一个值真正在编译期就能用, 还需要给变量加上 `constexpr` (或 `const` + 编译期初始化)

### cooked 字面值 vs raw 字面值

用户自定义字面值有两种形式

- **cooked (已处理)**: 编译器把字面量先按内置规则解析, 再传给你的 operator. 大多数场景用这种, 例如 `operator"" _km(long double)` 收到的就是已经被解析好的浮点数
- **raw (原始)**: 编译器把字面量的**原始字符序列**传给你, 由你自己解析. 写法是 `operator"" _suf(const char* str)`, 适合需要绕过内置规则的特殊场合 (例如自定义大整数解析)

```cpp
// cooked: 收到 long double
constexpr long double operator"" _km(long double v) { return v * 1000.0L; }

// raw: 收到 "1500" 这串字符
constexpr long long operator"" _bigint(const char* str) {
    long long n = 0;
    for (auto p = str; *p; ++p) n = n * 10 + (*p - '0');
    return n;
}
```

### 析构函数: C++11 要平凡, C++20 放宽到 constexpr

C++11 的字面值类型要求析构函数必须是 **平凡的** (trivial). 这就是为什么早期 `std::string` 不是字面值类型 - 它要在析构时释放堆内存. C++20 把这条放宽到 "析构可以是 constexpr", 所以 C++20 之后 `std::string` 也能进 constexpr 上下文了

```cpp
// C++11 / C++17 中 std::string 不是字面值类型
constexpr std::string s = "abc"; // C++17 下报错
```

### constexpr 构造函数不能是拷贝 / 移动构造

LiteralType 要求至少有一个 **不是拷贝 / 移动** 的 constexpr 构造函数. 也就是说光给拷贝构造加 constexpr 是不够的, 必须有一个 "能从原始数据构造出对象" 的 constexpr 构造

## 三、练习代码

### 练习代码主题

- 0 - [理解字面值类型的概念 - 哪些能 constexpr](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/08-literal-type-0.cpp)
- 1 - [自定义字面值类型 - 给 Vector 加 constexpr 构造](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/08-literal-type-1.cpp)

### 练习代码自动检测命令

```
d2x checker literal-type-0
d2x checker literal-type-1
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
