<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./15-variadic-templates.html
[English]: ../en/cpp11/15-variadic-templates.html

# 可变参数模板 - variadic templates

可变参数模板是 C++11 引入的一项核心模板特性, 允许函数模板和类模板接受 **任意数量、任意类型** 的参数, 让 `printf` 风格的多参数接口在 C++ 中第一次具备了类型安全和编译期可检查的实现方式

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/parameter_pack) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/15-variadic-templates.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/15-variadic-templates-0.cpp) |  |


**为什么引入?**

- C++11 之前处理任意数量参数只能依赖 C 风格的可变参数宏 (`...` / `__VA_ARGS__`) 或者重载 / 宏生成大量模板, 既不类型安全也难以维护
- 标准库需要一种统一的机制实现 `make_shared`, `tuple`, `function` 等可变模板组件
- 与右值引用 / 完美转发结合, 才能真正写出 "通用、零开销" 的转发型接口

**和 C 风格可变参数 / 模板硬展开 的区别?**

- 可变参数宏 (`__VA_ARGS__`) 只是文本替换, 没有类型信息, 也无法在运行期遍历参数 -- 错一个格式符就崩溃
- 模板重载硬编码 (1, 2, 3, ... N 个参数各写一份) 代码冗余、扩展性差, 通常还要靠宏批量生成
- 可变参数模板由编译器在编译期展开参数包, 完整保留每个参数的 **类型 / 引用类别 / cv 限定**, 并能与 `std::forward` 协作完成完美转发

## 一、基础用法和场景

### 历史背景 - C++11 之前的方案

C++11 引入可变参数模板之前, 通常使用 **可变参数宏** 或 **模板重载 + 宏生成** 处理可变参数. 这两种方案都存在明显的缺陷, 这里以实现一个支持任意参数的输出函数为例对比

**可变参数宏**

继承自 C 语言, 使用 `...` 表示宏定义的可变参数, 使用 `__VA_ARGS__` 访问宏调用时传入的参数

```cpp
#define LOG(fmt, ...)  printf(fmt, __VA_ARGS__)

LOG("x = %d, y = %f\n", 10, 3.14); // 展开为 printf("x = %d, y = %f\n", 10, 3.14);
LOG("Hello");                       // 展开为 printf("Hello");
```

可变参数宏使用简单但场景受限, 难以与现代 C++ 代码结合:

- 无法实现类型安全检查
  - `LOG("%s", 42); // 编译通过, 但运行时崩溃或输出垃圾`
- 无法处理引用 / 移动语义、无法保存参数包
- 无法遍历 `__VA_ARGS__` 并对每个参数做不同处理

**模板重载与硬编码**

最直观也最笨拙的方法 -- 手动为 1 个、2 个、... N 个参数分别编写重载版本. 代码冗余极大, 维护和扩展都很困难

**宏与模板生成**

`Boost.Preprocessor` 广泛使用这种技巧 -- 让编译器自动生成 `template<typename T1> void print(T1)`, `template<typename T1, typename T2> void print(T1, T2)`, ... 对应的重载

```cpp
#define TP_PARAM(n)    typename T##n
#define FN_PARAM(n)    T##n p##n
#define PRINT_BODY(n)  std::cout << p##n << " ";

#define REPEAT_TP_3     typename T1, typename T2, typename T3
#define REPEAT_FN_3     T1 p1, T2 p2, T3 p3
#define REPEAT_PRINT_3  PRINT_BODY(1) PRINT_BODY(2) PRINT_BODY(3)

#define DEFINE_LOG_FUNCTION(n) \
    template<REPEAT_TP_##n> \
    void log(REPEAT_FN_##n) { REPEAT_PRINT_##n std::cout << std::endl; }

DEFINE_LOG_FUNCTION(3)
// 展开为:
// template <typename T1, typename T2, typename T3>
// void log(T1 p1, T2 p2, T3 p3) {
//     std::cout << p1 << " "; std::cout << p2 << " "; std::cout << p3 << " ";
//     std::cout << std::endl;
// }
```

这种方式扩展性差、编译速度慢, 也不便调试 -- 这正是可变参数模板要解决的问题

### 模板参数包和函数参数包的语法

C++11 用 `...` 在模板中表示参数包. 以最经典的 `print` 为例:

```cpp
// 递归终止函数
void print() { std::cout << std::endl; }

// 展开参数包的模板
template<typename T, typename... Args>
void print(T first, Args... args) {
    std::cout << first << " ";
    print(args...); // 递归调用, 每次剥离一个参数
}

// print(1, "Hello", 3.14, 'A'); // 完美运行, 类型安全
```

`...` 在 **三个位置** 出现, 含义都不同:

1. `template<typename T, typename... Args>` -- 修饰 `typename`, 表示 **模板参数包** (类型可变)
2. `void print(T first, Args... args)` -- 修饰 `Args`, 表示 **函数参数包** (函数形参列表可变)
3. `print(args...)` -- 修饰参数名, 表示 **参数包展开** (在调用点把参数依次铺开)

### 参数包的展开 - 递归剥离

C++11 没有直接遍历参数包的语法, 通常采用递归方式 -- 每次模板调用从参数包中 "剥" 出一个参数处理, 剩余的继续往下递归. 递归终止函数通常为 **同名的非模板函数** (重载决议时优先使用非模板版本) 或 **单参数模板特化**

```cpp
// 终止: 单参数版本
template<typename T>
T sum(T x) { return x; }

// 展开: 多参数版本
template<typename T, typename... Args>
T sum(T first, Args... args) {
    return first + sum(args...);
}
```

### 与完美转发结合 - make_shared

可变参数模板真正强大的地方在于 **和万能引用 + 完美转发结合**, 把任意参数原样转发给目标构造函数. 标准库里的 `std::make_shared` 就是教科书式的用法

```cpp
template <typename T, typename... Args>
std::shared_ptr<T> make_shared(Args&&... args) {
    T* ptr = new T(std::forward<Args>(args)...);
    return std::shared_ptr<T>(ptr);
}
```

`Args&&...` 是 "参数包形式的万能引用", `std::forward<Args>(args)...` 在展开时为参数包中每一个元素分别套上对应的 `std::forward`, 完整保留每个参数的左值 / 右值类别

### sizeof... 与 C++14 的 index_sequence

C++14 没有给可变参数模板加新语法, 但通过 `std::index_sequence` / `std::make_index_sequence` 提供了 **不靠递归** 也能展开参数包的能力 -- 在编译期生成 `0, 1, ..., N-1` 的整数序列, 然后用一次模板特化把序列展开

```cpp
template <typename T, std::size_t... Is>
void print_impl(T&& t, std::index_sequence<Is...>) {
    // 用逗号表达式 + 初始化列表展开参数包, 逐个打印
    using expander = int[];
    (void)expander{ 0,
        ((std::cout << "Arg " << Is << ": "
                    << std::get<Is>(std::forward<T>(t)) << '\n'), 0)... };
}

template <typename... Args>
void print_args(Args&&... args) {
    auto t = std::make_tuple(std::forward<Args>(args)...);
    print_impl(t, std::make_index_sequence<sizeof...(Args)>{});
}
```

`sizeof...(Args)` 返回参数包中元素的 **个数**, 是写可变参数模板时几乎必用的运算符

### C++17 折叠表达式 - 取代递归

C++17 引入 **折叠表达式 (Fold Expressions)**, 把递归展开换成更直观的语法, 用一个二元运算符把参数包整个 "折" 起来, 极大减少了样板代码. 折叠表达式有四种形式:

- 一元左折叠: `(... op pack)` -- `((p1 op p2) op p3) op ...`
- 一元右折叠: `(pack op ...)` -- `p1 op (p2 op (p3 op ...))`
- 二元左折叠: `(init op ... op pack)` -- 比一元多一个初值
- 二元右折叠: `(pack op ... op init)`

```cpp
// 一元右折叠 - 求和
template <typename... Args>
auto sum(Args... args) { return (args + ...); }

// 一元左折叠 - 减法 (注意求值顺序)
template <typename... Args>
auto sub(Args... args) { return (... - args); }

// 二元左折叠 - 带初值的减法: (((init - a1) - a2) - ... - aN)
template<typename T, typename... Args>
auto sub_with_init_left(T init, Args... args) { return (init - ... - args); }
```

配合 `if constexpr`, 还能在编译期对参数包做条件分支, 不再需要专门写一个空的递归终止函数

```cpp
template<typename T, typename... Args>
void process_args(T first_arg, Args... rest_args) {
    process_value(first_arg);
    if constexpr (sizeof...(rest_args) > 0) { // 编译期检查
        process_args(rest_args...);
    }
}
```

## 二、注意事项

### 递归终止函数必须独立可见

C++11 风格的递归展开依赖重载决议在 "继续递归" 和 "终止递归" 之间挑选合适的版本. 终止函数 (空参数或单参数) 必须 **在递归模板之前可见**, 否则会出现找不到匹配的编译错误

### 递归深度受编译器限制

每剥离一个参数就多一层模板实例化, 编译器默认的实例化深度上限通常是 1024 (可通过 `-ftemplate-depth=N` 调高). 参数极多 / 嵌套极深的展开建议改用 `index_sequence` 或 C++17 折叠表达式, 既快又浅

### 完美转发参数包的固定写法

参数包形式的万能引用必须写成 `Args&&... args`, 转发时必须写成 `std::forward<Args>(args)...` -- 把 `forward<Args>(args)` 整体作为模式, 末尾的 `...` 才会对每个元素分别展开. 写成 `std::forward<Args...>(args...)` 是常见错误

### 参数包不能直接被 "保存"

参数包本身不是一等公民, 不能赋值给某个变量留到以后用. 常见做法是把它打包进 `std::tuple`, 之后用 `std::index_sequence` 展开消费

```cpp
template <typename... Args>
auto save(Args&&... args) {
    return std::make_tuple(std::forward<Args>(args)...); // 打包
}
```

### 优先用 C++17 折叠表达式 / 标准库设施

如果项目允许 C++17, 写新代码应该首选折叠表达式 + `if constexpr`, 避免 "终止函数 + 递归" 的样板. C++11 风格的递归展开主要在 **维护老代码** 或 **限定 C++11 标准** 时才需要

## 三、练习代码

### 练习代码主题

- 0 - [可变参数模板基础 - 递归展开 print](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/15-variadic-templates-0.cpp)
- 1 - [可变参数模板求和 - sum](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/15-variadic-templates-1.cpp)

### 练习代码自动检测命令

```
d2x checker variadic-templates
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
