<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./06-scoped-enums.html
[English]: ../en/cpp11/06-scoped-enums.html

# 强类型枚举 - scoped enums

scoped enum (`enum class` / `enum struct`) 是 C++11 引入的强类型枚举, 用于解决传统 `enum` 名字泄漏到外层作用域、隐式转换为 int、底层类型不可控等问题, 让枚举值真正成为一个独立的、类型安全的离散集合

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/enum) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/06-scoped-enums.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/06-scoped-enums-0.cpp) |  |


**为什么引入?**

- 传统 enum 的枚举值会泄漏到外层作用域, 容易和其他名字冲突
- 传统 enum 会隐式转换成 int, 容易出现不安全的算术 / 比较
- 无法显式指定底层类型, 跨平台 / 跨编译器尺寸不确定

**和传统 enum 的区别?**

- enum class 的枚举值不会泄漏到外层作用域, 必须通过 `EnumName::Value` 访问, 同名值在不同枚举中可以共存
- enum class 不会隐式转换为整型, 想要数值时必须用 `static_cast`, 不同枚举之间也不能互相比较
- enum class 可以显式指定底层类型 (例如 `: uint8_t`), 内存布局可控, 同时也支持只声明不定义的 **前向声明**

## 一、基础用法和场景

### enum class 的基本语法

`enum class` 是 scoped enum 的关键字组合 (`enum struct` 等价), 在传统 `enum` 后面加 `class` 即可

```cpp
enum class Color {
    RED,
    GREEN,
    BLUE,
    ORANGE
};

enum class Fruit {
    Apple,
    Banana,
    ORANGE // 和 Color::ORANGE 同名也没关系, 各自有独立作用域
};
```

如果换成传统 enum, 上面这两份定义放在同一作用域里就会因为 `ORANGE` 重复定义而编译失败

### 显式作用域 - 通过 EnumName::Value 访问

scoped enum 的枚举值不会暴露到外层, 访问时必须带上枚举名作为前缀

```cpp
Color color = Color::ORANGE; // ok
Fruit fruit = Fruit::ORANGE; // ok, 和 Color::ORANGE 是两个不同的值

// Color c = ORANGE; // 错误: ORANGE 在当前作用域里不存在
```

这种 "强制带前缀" 的访问方式让阅读代码时能立刻看出某个常量属于哪个枚举, 也彻底消除了符号冲突

### 不再隐式转换为 int - 比较和算术更安全

传统 enum 会隐式转成 int, 所以下面这种 "颜色 == 水果" 的比较会被静默接受

```cpp
enum Color { RED, GREEN, BLUE };
enum Fruit { Apple, Banana };

Color c = RED;
if (c == Apple) { /* 编译通过! 实际是 0 == 0, 永远成立 */ }
```

scoped enum 直接在编译期把这种错误拦下来

```cpp
enum class Color { RED, GREEN, BLUE };
enum class Fruit { Apple, Banana };

Color c = Color::RED;
// if (c == Fruit::Apple) { } // 错误: 不同枚举类型不能比较
// int n = c;                 // 错误: 不能隐式转 int
int n = static_cast<int>(c);  // ok: 必须显式 cast
```

同一个 `enum class` 内部的两个值之间可以用 `==` / `!=` 比较, 但和其他枚举、和整型之间的比较都会被拒绝

### 显式指定底层类型 - enum class X : uint8_t { ... }

scoped enum 默认底层类型是 `int`, 可以在枚举名后加 `: 类型` 来显式指定, 从而精确控制内存占用

```cpp
enum class Color {           // 默认底层类型是 int
    RED, GREEN, BLUE
};

enum class Color8Bit : int8_t {   // 显式指定为 int8_t
    RED, GREEN, BLUE, ORANGE
};

static_assert(sizeof(Color)     == sizeof(int),    "");
static_assert(sizeof(Color8Bit) == sizeof(int8_t), "");
```

枚举值也可以显式指定数值, 没指定的部分会从前一个值 +1 顺延

```cpp
enum class ErrorCode : int {
    OK      = 0,
    ERROR_1,        // 1
    ERROR_2 = -2,
    ERROR_3 = 3     // 显式指定为 3
};

static_cast<int>(ErrorCode::ERROR_3); // 3
```

配合协议、网络包、嵌入式寄存器这类对内存布局敏感的场景非常有用

### 前向声明的支持

由于 scoped enum 的底层类型在声明时就已确定 (默认 int 或显式指定), 所以可以只声明而不给出枚举值列表, 实现前向声明

```cpp
// header
enum class Status : uint8_t;       // 前向声明 ok

void handle(Status s);             // 接口里就能用上

// .cpp
enum class Status : uint8_t {
    Ok, Pending, Failed
};
```

传统 enum 因为底层类型要靠枚举值范围推断, 所以不能这样前向声明 (除非也显式指定底层类型, 但那已经是 C++11 之后的扩展用法)

## 二、注意事项

### 当你确实需要数值时, 用 static_cast

scoped enum 不会自动转 int, 一旦需要把枚举值喂给数组下标、序列化、日志、整型 API, 都必须显式转换

```cpp
enum class Color { RED, GREEN, BLUE };

Color c = Color::GREEN;

// int idx = c;                       // 错误
int idx = static_cast<int>(c);        // ok

std::cout << static_cast<int>(c);     // ok: 否则 << 也找不到匹配的重载
```

反过来, 从整型构造枚举值同样要显式 cast: `Color c = static_cast<Color>(1);`. 这一步是有意为之的 "摩擦力", 提醒你确认这次转换是必要且安全的

### scoped enum 不能直接用作位掩码 - 要么 cast 要么定义 operator|

传统 enum 经常被当成位标志, 因为可以直接 `FLAG_A | FLAG_B`. scoped enum 因为没有隐式转 int, 这种写法是不通的

```cpp
enum class Perm : uint32_t {
    Read  = 1 << 0,
    Write = 1 << 1,
    Exec  = 1 << 2
};

// auto p = Perm::Read | Perm::Write; // 错误: 没有 operator|
```

两种常见的处理方式:

```cpp
// 方式 1: 在使用点 cast
auto p = static_cast<Perm>(
    static_cast<uint32_t>(Perm::Read) |
    static_cast<uint32_t>(Perm::Write)
);

// 方式 2: 给这个枚举重载 operator|
constexpr Perm operator|(Perm a, Perm b) {
    return static_cast<Perm>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
    );
}
```

方式 2 之后 `Perm::Read | Perm::Write` 就能正常使用, 同时还保留了类型安全

### 传统 enum 的迁移建议

老代码里大量使用传统 enum 是常见情况, 不必一次全部改写, 可以按这些策略逐步收敛:

- 新代码默认用 `enum class`
- 改老代码时优先处理 "明显容易冲突的命名" 和 "依赖隐式转 int 的可疑比较"
- 对内存布局敏感的枚举, 顺手补上 `: uint8_t` / `: uint16_t` 等显式底层类型
- 需要位标志语义的枚举, 选择 cast 或重载 `operator|` / `operator&`, 不要回退到传统 enum

## 三、练习代码

### 练习代码主题

- 0 - [传统 enum 的潜在问题 - 名字冲突与隐式比较](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/06-scoped-enums-0.cpp)
- 1 - [scoped enum 基本用法 - 作用域 / 类型安全 / 底层类型](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/06-scoped-enums-1.cpp)

### 练习代码自动检测命令

```
d2x checker scoped-enums
d2x checker scoped-enums-1
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
