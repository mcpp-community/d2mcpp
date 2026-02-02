<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/13-long-long.html
[English]: ./13-long-long.html

# long long - 64-bit Integer Type

`long long` is a **64-bit integer type** introduced in C++11, used to represent larger range integer values. It solves the range limitation issues of traditional integer types when representing large integers.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/types) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/13-long-long.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/13-long-long-0.cpp) |  |

**Why was it introduced?**

- Solve the insufficient range of traditional integer types
- Provide a unified 64-bit integer type standard

**What's the difference between long long and traditional integer types?**

- `long long` guarantees at least 64-bit width, with range at least from -2^63 to 2^63-1
- `int` is typically 32-bit, with range approximately -2.1 billion to 2.1 billion
- `long` is 32-bit on 32-bit systems, typically 64-bit on 64-bit systems (but standard only guarantees at least 32-bit)

## I. Basic Usage and Scenarios

### Basic Declaration and Initialization

> Support for signed and unsigned versions, with literal suffixes

```cpp
// Signed long long
long long val1 = 1;
long long val2 = -1;

// Unsigned long long
unsigned long long uVal1 = 1;

// Literal identifiers + type deduction
auto longlong = 1LL;
auto ulonglong = 1ULL;
```

### Large Integer Applications and Boundary Values

> Handle calculations beyond traditional integer type ranges, based on boundary value acquisition

```cpp
//#include <limits>

// Using long long for large number calculations (exceeding int range)
long long population = 7800000000LL;  // World population

// Get integer type boundaries
int maxInt = std::numeric_limits<int>::max();
long long maxLL = std::numeric_limits<long long>::max();
auto minLL = std::numeric_limits<long long>::min();
```

## II. Important Notes

### Type Deduction and Literal Suffixes

Use `LL` or `ll` suffix to explicitly specify `long long` literals, use `ULL` or `ull` to specify unsigned versions

```cpp
auto num1 = 10000000000;    // Type may be int or long, depending on compiler
auto num2 = 10000000000LL;  // Explicitly long long to assist type deduction
```

### Type Conversion and Precision Issues

Be aware of precision loss that may occur during conversions between different integer types

```cpp
long long bigValue = 3000000000LL;
int smallValue = bigValue;  // May overflow

std::cout << "bigValue: " << bigValue << std::endl;
std::cout << "smallValue: " << smallValue << std::endl;  // May be incorrect

// Safe conversion check
if (bigValue > std::numeric_limits<int>::max() || bigValue < std::numeric_limits<int>::min()) {
    std::cout << "Conversion would cause overflow!" << std::endl;
}
```

### Bit-Width Confusion - Why Doesn't the Standard Fix the Bit Width?

**Reasons**

- **Hardware Variations:** Different architectures have different "natural word sizes," such as 16/32/64 bits, and many embedded systems only support 8/16-bit multiplication and division instructions. If `long` were forcibly defined as 64 bits, it would cause significant performance issues on some machines (e.g., 32-bit MCUs).
  - For example: Performing 64-bit calculations on an 8-bit machine without relevant hardware instructions would require algorithmic simulation, leading to a sharp increase in `instruction cycles`.
- **Historical and ABI Compatibility:** C/C++ predates the widespread adoption of modern 32/64-bit systems. Many platforms have system interfaces, file formats, and calling conventions that have already encoded the size of `int`/`long` into their ABI. Forcing a change in the standard would break binary compatibility and disrupt the ecosystem.
- **Zero-Cost Abstraction:** The C/C++ standard is designed to map efficiently to the underlying hardware. It only specifies behavior and minimum ranges, allowing implementations to choose the most natural width for the platform, thereby achieving zero-cost or near-zero-cost abstraction.

**Solutions**

- **Optional Fixed-Width Types in C/C++:** When precise bit widths are required, use types from `<cstdint>`/`<stdint.h>` such as `int8_t`, `int16_t`, `int32_t`, `int64_t`, etc.
- **Avoid Bit-Width Assumptions and Use Static Assertions:** Avoid assuming the bit width of types during development to improve portability. If certain code relies on specific bit-width assumptions, use static assertions to ensure the width meets expectations: `static_assert(sizeof(T) == N)`.


![](../../imgs/long-long.0.png)

## III. Practice Code

### Practice Code Topics

- 0 - [long long Basic Usage](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/13-long-long-0.cpp)
- 1 - [long long Large Number Applications and Boundary Values](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp11/13-long-long-1.cpp)

### Auto-Checker Command

```bash
d2x checker long-long
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://xlings.d2learn.org)
