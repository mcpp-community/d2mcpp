<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp14/08-aggregate-nsdmi.html
[English]: ./08-aggregate-nsdmi.html

# Aggregate NSDMI Initialization

C++14 relaxes aggregate restrictions — classes with non-static data member initializers (NSDMI) can still be aggregates

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-aggregate_initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/08-aggregate-nsdmi.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/08-aggregate-nsdmi-0.cpp) |  |


**Why introduced?**

In C++11, a class with any NSDMI (`int x = 0;`) lost its aggregate status, breaking `{}` initialization. C++14 fixes this contradiction

## I. Basic Usage and Scenarios

```cpp
struct Point {
    int x = 0;  // NSDMI
    int y = 0;
};

Point p1{3, 4};   // C++14: aggregate initialization OK
Point p2{1};      // y uses default 0
Point p3{};       // x=0, y=0
```

## II. Real-World Case — Aggregate + NSDMI in the STL

> The MSVC STL uses aggregates with NSDMI internally. The example below cites the vendored [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) (source: [`msvc-stl/stl/inc/array`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/array#L20-L23))

```cpp
// MSVC STL · msvc-stl/stl/inc/array (abridged)
struct _Iterator_base12_compatible {
    _Container_proxy* _Myproxy{};
    _Iterator_base12* _Mynextiter{};
};
```

Both members have `{}` default initializers, yet the type participates in iterator base class inheritance and initialization as an aggregate — relying on C++14's NSDMI aggregate relaxation

## III. Notes

- NSDMI values are only used when no initializer is provided
- `{}` initialization values override NSDMI defaults
- Aggregate members must all be public

## IV. Exercise Code

### Exercise Topics

- 0 - [Aggregate NSDMI — Class with Default Members and Aggregate Init](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/08-aggregate-nsdmi-0.cpp)

### Auto-Checker Command

```
d2x checker aggregate-nsdmi
```

## V. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
