<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp14/07-deprecated-attribute.html
[English]: ./07-deprecated-attribute.html

# deprecated Attribute

C++14 introduces the `[[deprecated]]` attribute to mark deprecated functions, classes, or variables, producing compile-time warnings

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-attribute](https://en.cppreference.com/w/cpp/language/attributes) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/07-deprecated-attribute.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/07-deprecated-attribute-0.cpp) |  |


**Why introduced?**

- Before C++11, there was no standard way to mark deprecated APIs — only documentation or non-standard `#warning`
- `[[deprecated]]` produces warnings at compile time that callers cannot ignore

## I. Basic Usage and Scenarios

```cpp
[[deprecated("Use new_api() instead")]]
void old_api() { }

[[deprecated]]
int legacy_value = 42;

void modern_code() {
    old_api();       // warning: old_api is deprecated
    int x = legacy_value;  // warning: legacy_value is deprecated
}
```

## II. Real-World Case — [[deprecated]] in the STL

> The MSVC STL wraps `[[deprecated]]` in macros for deprecation warnings on obsolete headers. The example below cites the vendored [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) (source: [`msvc-stl/stl/inc/yvals_core.h`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/yvals_core.h#L1057-L1061))

```cpp
// MSVC STL · msvc-stl/stl/inc/yvals_core.h (abridged)
#define _CXX17_DEPRECATE_C_HEADER \
    [[deprecated("warning STL4004: " \
                 "<ccomplex>, <cstdalign>, <cstdbool>, and <ctgmath> " \
                 "are deprecated in C++17.")]]
```

## III. Notes

- Can mark: functions, classes, variables, enums, using aliases
- Message string is optional but recommended
- Deprecated does not mean removed — the compiler still generates code

## IV. Exercise Code

### Exercise Topics

- 0 - [[[deprecated]] Attribute — Marking Deprecated Functions](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/07-deprecated-attribute-0.cpp)

### Auto-Checker Command

```
d2x checker deprecated-attribute
```

## V. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
