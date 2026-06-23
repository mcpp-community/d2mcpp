<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp14/06-digit-separators.html
[English]: ./06-digit-separators.html

# Digit Separators

C++14 allows single quotes `'` as separators in numeric literals, improving readability without affecting the value

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-integer_literal](https://en.cppreference.com/w/cpp/language/integer_literal) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/06-digit-separators.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/06-digit-separators-0.cpp) |  |


**Why introduced?**

`1000000000` is hard to parse, `0xFFFF0000` requires counting Fs. `1'000'000'000` and `0xFFFF'0000` make the numeric structure clear at a glance

## I. Basic Usage and Scenarios

```cpp
int million  = 1'000'000;          // decimal
int hex_val  = 0xFF'FF'00'00;       // hexadecimal
int bin_val  = 0b1010'1100'1111;    // binary + separator

static_assert(million == 1000000, "");
```

## II. Real-World Case — Digit Separators in the STL

> The MSVC STL uses binary literals with digit separators in UTF-8 decoding. The example below cites the vendored [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) (source: [`msvc-stl/stl/inc/format`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/format#L281-L282))

```cpp
// MSVC STL · msvc-stl/stl/inc/format (abridged)
// UTF-8 decoding — extract the lower 6 bits of each continuation byte
_Val = (_Val << 6) | (static_cast<unsigned char>(_First[_Idx]) & 0b11'1111u);
```

`0b11'1111u` is 63, and the `'` separator makes the binary mask's semantics directly visible in the literal — no mental conversion from `0x3F` or `63`

## III. Notes

- Cannot appear at the start or end: `'0` / `1'` are invalid
- Cannot be consecutive: `1''0` is invalid
- Does not affect the value, only improves readability

## IV. Exercise Code

### Exercise Topics

- 0 - [Digit Separators — Large Numbers and Radix Combinations](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/06-digit-separators-0.cpp)

### Auto-Checker Command

```
d2x checker digit-separators
```

## V. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
