<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./06-digit-separators.html
[English]: ../en/cpp14/06-digit-separators.html

# 数字分隔符 - digit separators

C++14 允许在数字字面量中使用单引号 `'` 作为分隔符, 不影响数值, 显著提升大数可读性

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-integer_literal](https://en.cppreference.com/w/cpp/language/integer_literal) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp14/06-digit-separators.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/06-digit-separators-0.cpp) |  |


**为什么引入?**

`1000000000` 很难分清位数, `0xFFFF0000` 需要仔细数几位 F。`1'000'000'000` 和 `0xFFFF'0000` 让数值结构一目了然

## 一、基础用法和场景

```cpp
int million  = 1'000'000;          // 十进制
int hex_val  = 0xFF'FF'00'00;       // 十六进制
int bin_val  = 0b1010'1100'1111;    // 二进制 + 分隔符

static_assert(million == 1000000, "");
```

分隔符可以出现在数字中任意位置, 不限制分组位数

## 二、真实案例 - STL 中的数字分隔符

> MSVC STL 在 UTF-8 解码中使用二进制字面量配合数字分隔符。下面以仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 为例 (源码: [`msvc-stl/stl/inc/format`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/format#L281-L282))

```cpp
// MSVC STL · msvc-stl/stl/inc/format (有删节)
// UTF-8 解码 — 提取连续字节的低 6 位
_Val = (_Val << 6) | (static_cast<unsigned char>(_First[_Idx]) & 0b11'1111u);
```

`0b11'1111u` 是 63, 用 `'` 将高位和低位分开后, 二进制掩码的语义直接从字面量可见, 无需心算 `0x3F` 或 `63`

## 三、注意事项

- 不能出现在数字开头或结尾: `'0` / `1'` 非法
- 不能连续: `1''0` 非法
- 不影响数值, 仅增强可读性

## 四、练习代码

### 练习代码主题

- 0 - [数字分隔符 — 大数与进制组合](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/06-digit-separators-0.cpp)

### 练习代码自动检测命令

```
d2x checker digit-separators
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
