<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp14/03-return-type-deduction.html
[English]: ./03-return-type-deduction.html

# Return Type Deduction

C++14 allows functions to use `auto` as a return type without trailing `-> decltype(...)`

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-auto](https://en.cppreference.com/w/cpp/language/auto) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/03-return-type-deduction.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/03-return-type-deduction-0.cpp) |  |


**Why introduced?**

- C++11 required `-> decltype(...)` trailing return for `auto` functions
- Many simple functions don't need an explicit return type

## I. Basic Usage and Scenarios

### auto return — plain functions

```cpp
auto add(int a, int b) {    // deduced as int
    return a + b;
}
```

### auto return — template functions

```cpp
template <typename T1, typename T2>
auto multiply(T1 a, T2 b) {  // C++14: no -> decltype needed
    return a * b;
}
```

## II. Real-World Case — auto Return Deduction in the STL

> The MSVC STL uses `auto` return types extensively to simplify template function signatures. The examples below cite the vendored [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) (source: [`msvc-stl/stl/inc/xutility`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/xutility#L1129-L1132)); `_NODISCARD` / `constexpr` are library-internal annotations

### _To_unsigned_like — auto Return Simplifies Template Functions

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (abridged)
template <_Integer_like _Ty>
_NODISCARD constexpr auto _To_unsigned_like(const _Ty _Value) noexcept {
    return static_cast<_Make_unsigned_like_t<_Ty>>(_Value);
}
```

The return type is `_Make_unsigned_like_t<_Ty>` — without `auto`, the signature would be `_Make_unsigned_like_t<_Ty> _To_unsigned_like(...)`, with the return type longer than the function name. `auto` keeps the signature clean while the return statement naturally expresses the type

## III. Notes

### auto return requires consistent types

```cpp
auto bad(int x) {
    if (x > 0) return 1;       // int
    else       return 2.0;     // double → error!
}
```

### Recursion requires explicit return type

```cpp
auto factorial(int n) {             // error: recursive
    return n <= 1 ? 1 : n * factorial(n - 1);
}
```

### auto return strips references

Use `decltype(auto)` when you need to preserve references (see next chapter)

## IV. Exercise Code

### Exercise Topics

- 0 - [auto Return Type Deduction](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/03-return-type-deduction-0.cpp)

### Auto-Checker Command

```
d2x checker return-type-deduction
```

## V. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
