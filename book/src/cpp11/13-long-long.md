<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./13-long-long.html
[English]: ../en/cpp11/13-long-long.html

# long long - 64位整数类型

`long long` 是C++11引入的**64位整数类型**，用于表示更大范围的整数值。它解决了传统整数类型在表示大整数时的范围限制问题。

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/types) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/13-long-long.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/src/cpp11/tests/13-long-long/0.cpp) |  |

**为什么引入?**

- 解决传统整数类型范围不足的问题
- 提供统一的64位整数类型标准

**long long和传统整数类型有什么区别?**

- `long long` 保证至少64位宽度，范围至少为 -2^63 到 2^63-1
- `int` 通常为32位，范围约为 -21亿到21亿
- `long` 在32位系统上为32位，在64位系统上通常为64位（但标准只保证至少32位）

## 一、基础用法和场景

### 基本声明和初始化

> 支持有符号和无符号, 以及字面量后缀标识

```cpp
// 有符号long long
long long val1 = 1;
long long val2 = -1;

// 无符号long long
unsigned long long uVal1 = 1;

// 字面量标识 + 类型推导
auto longlong = 1LL:
auto ulonglong = 1ULL;
```

### 大整数应用和边界值

> 处理超出传统整数类型范围的计算，基于边界值获取

```cpp
//#include <limits>

// 使用long long处理大数计算(超过int表示范围)
long long population = 7800000000LL;  // 世界人口

// 获取整数类型边界
int maxInt = std::numeric_limits<int>::max();
long long maxLL = std::numeric_limits<long long>::max();
auto minLL = std::numeric_limits<long long>::min();
```

## 二、注意事项

### 类型推导和字面量后缀

使用`LL`或`ll`后缀明确指定`long long`字面量，使用`ULL`或`ull`指定无符号版本

```cpp
auto num1 = 10000000000;    // 类型可能是int或long，取决于编译器
auto num2 = 10000000000LL;  // 明确为long long辅助类型推导
```

### 类型转换和精度问题

注意不同整数类型之间的转换可能导致的精度损失

```cpp
long long bigValue = 3000000000LL;
int smallValue = bigValue;  // 可能溢出

std::cout << "bigValue: " << bigValue << std::endl;
std::cout << "smallValue: " << smallValue << std::endl;  // 可能不正确

// 安全转换检查
if (bigValue > std::numeric_limits<int>::max() || bigValue < std::numeric_limits<int>::min()) {
    std::cout << "转换会导致溢出!" << std::endl;
}
```

### 位宽疑惑 - 标准中为什么不固定位宽?

**原因**

- **硬件差异问题:** 不同架构“自然字长”不同：16/32/64 位都有，大量嵌入式甚至只有 8/16 位乘除指令.若强行规定(long为64位)，一些机器(32 位 MCU）上会造成巨大的性能损失
  - 例如: 在8位机器上做64位计算, 但没有相关的机器指令。所以需要通过算法模拟的方式实现, 进而导`指令周期`攀升
- **历史与 ABI 兼容:** C/C++ 起源早于现代 32/64 位普及，许多平台的系统接口、文件格式、调用约定都已把 int/long 的大小写进了 ABI。标准若强制改变，会破坏二进制兼容和生态
- **零成本抽象:** C/C++ 标准面向“与机器高效映射”的抽象机，只规定行为与最小范围，让实现能选择对该平台最自然的宽度，从而获得零开销抽象或接近零开销

**解决方案**

- **C/C++提供了可选方案:** 需要精确位宽时，可以使用`<cstdint>/<stdint.h>`里的 `int8_t`、`int16_t`、`int32_t`、`int64_t`...
- **不假设位宽和静态断言:** 开发时不假设类型的位宽, 从而提高可移植性. 如果部分代码做了位宽假设, 可以通过静态断言来保证位宽符合预期 `static_assert(sizeof(T)==N)`


![](../imgs/long-long.0.png)

## 三、练习代码

### 练习代码主题

- 0 - [long long基础用法](https://github.com/mcpp-community/d2mcpp/blob/main/src/cpp11/tests/13-long-long/0.cpp)
- 1 - [long long大数应用和边界值](https://github.com/mcpp-community/d2mcpp/blob/main/src/cpp11/tests/13-long-long/1.cpp)

### 练习代码自动检测命令

```bash
d2x checker long-long
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://xlings.d2learn.org)
