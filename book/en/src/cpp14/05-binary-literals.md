<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp14/05-binary-literals.html
[English]: ./05-binary-literals.html

# Binary Literals

C++14 introduces the `0b` / `0B` prefix for binary integer literals, making bit-level values directly readable

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-integer_literal](https://en.cppreference.com/w/cpp/language/integer_literal) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/05-binary-literals.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/05-binary-literals-0.cpp) |  |


**Why introduced?**

Bit masks and flag bits expressed in decimal or hex cannot directly convey the binary bit layout. `0b0010'1100` makes every bit's meaning visible at a glance

## I. Basic Usage and Scenarios

```cpp
int a = 0b1010;       // 10
int b = 0B1111;       // 15

// Bit masks — binary representation is the most intuitive
constexpr unsigned READ  = 0b001;
constexpr unsigned WRITE = 0b010;
constexpr unsigned EXEC  = 0b100;

unsigned perm = 0b101;  // READ | EXEC
```

## II. Real-World Case — Binary Literals in the STL

> The MSVC STL uses binary literals for bit masks in Unicode handling. The example below cites the vendored [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) (source: [`msvc-stl/stl/inc/format`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/format#L259-L267))

```cpp
// MSVC STL · msvc-stl/stl/inc/format (abridged)
// UTF-8 encoding — mask out the effective bits per byte count
switch (_Num_bytes) {
case 2:
    _Val &= 0b1'1111u;   // 2 bytes: keep lower 5 bits
    break;
case 3:
    _Val &= 0b1111u;     // 3 bytes: keep lower 4 bits
    break;
case 4:
    _Val &= 0b111u;      // 4 bytes: keep lower 3 bits
}
```

## III. Notes

- Binary literals can only be used with integer types
- Can be combined with digit separators: `0b1010'1100`
- Both `0b` and `0B` work; `0b` is more common

## IV. Exercise Code

### Exercise Topics

- 0 - [Binary Literals — Bit Mask Operations](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/05-binary-literals-0.cpp)

### Auto-Checker Command

```
d2x checker binary-literals
```

## V. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
