<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./05-binary-literals.html
[English]: ../en/cpp14/05-binary-literals.html

# 二进制字面量 - binary literals

C++14 引入 `0b` / `0B` 前缀表示二进制字面量, 让位运算和底层编程中的数值表示更加直观

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-integer_literal](https://en.cppreference.com/w/cpp/language/integer_literal) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/05-binary-literals.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/05-binary-literals-0.cpp) |  |


**为什么引入?**

位掩码和标志位用十进制或十六进制表示时, 无法直接对应二进制位布局。`0b0010'1100` 让每一位的含义肉眼可见

## 一、基础用法和场景

```cpp
int a = 0b1010;       // 10
int b = 0B1111;       // 15

// 位掩码 — 二进制表示最直观
constexpr unsigned READ  = 0b001;
constexpr unsigned WRITE = 0b010;
constexpr unsigned EXEC  = 0b100;

unsigned perm = 0b101;  // READ | EXEC
```

## 二、真实案例 - STL 中的二进制字面量

> MSVC STL 在 Unicode 处理中使用二进制字面量表示位掩码, 配合数字分隔符更加清晰。下面以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 为例 (源码: [`msvc-stl/stl/inc/format`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/format#L259-L267))

```cpp
// MSVC STL · msvc-stl/stl/inc/format (有删节)
// UTF-8 编码 — 按字节数截取有效位
switch (_Num_bytes) {
case 2:
    _Val &= 0b1'1111u;   // 2 字节: 取低 5 位
    break;
case 3:
    _Val &= 0b1111u;     // 3 字节: 取低 4 位
    break;
case 4:
    _Val &= 0b111u;      // 4 字节: 取低 3 位
}
```

UTF-8 编码中不同长度字符的有效数据位数不同 (5/4/3 位), 二进制字面量让每一位掩码都对应文档规范中的位布局, 十进制 `31` / `15` / `7` 则完全丢失这层语义

## 三、注意事项

- 二进制字面量只能用于整数类型
- 可以和数字分隔符组合: `0b1010'1100`
- 前缀大小写均可, 推荐 `0b` (更常见)

## 四、练习代码

### 练习代码主题

- 0 - [二进制字面量 — 位掩码运算](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/05-binary-literals-0.cpp)

### 练习代码自动检测命令

```
d2x checker binary-literals
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
