<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp14/02-variable-templates.html
[English]: ./02-variable-templates.html

# Variable Templates

C++14 introduces variable templates — allowing variables themselves to be parameterized by types

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-variable_template](https://en.cppreference.com/w/cpp/language/variable_template) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/02-variable-templates.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/02-variable-templates-0.cpp) |  |


**Why introduced?**

- In C++11, templates could only be applied to functions and classes
- Type traits required some_trait<T>::value everywhere
- C++14 variable templates enable pi<double> / is_same_v<T, U> syntax

## I. Basic Usage and Scenarios

### Basic Variable Template

```cpp
template <typename T>
constexpr T pi = T(3.1415926535897932385);

double d = pi<double>;   // 3.1415926535897932385
float  f = pi<float>;    // 3.14159265f
```

### _v Type Traits

```cpp
template <typename T, typename U>
constexpr bool is_same_v = std::is_same<T, U>::value;
```

### Recursive Variable Templates

```cpp
template <int N>
constexpr int factorial_v = N * factorial_v<N - 1>;

template <>
constexpr int factorial_v<0> = 1;
```

## II. Real-World Case — Variable Templates in the STL

> The MSVC STL uses variable templates internally to replace the old `::value` trait pattern. The example below cites the vendored [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) (source: [`msvc-stl/stl/inc/xutility`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/xutility#L1458-L1462))

### _Is_iterator_v — Variable Templates Simplify Type Traits

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (abridged)
template <class _Ty, class = void>
constexpr bool _Is_iterator_v = false;

template <class _Ty>
constexpr bool _Is_iterator_v<_Ty, void_t<_Iter_cat_t<_Ty>>> = true;

// External code uses _Is_iterator_v<_Ty> directly, no ::value needed
template <class _Ty>
struct _Is_iterator : bool_constant<_Is_iterator_v<_Ty>> {};
```

`_Is_iterator_v` is a variable template — defaulting to false, with a specialization to true when `_Ty` has an `_Iter_cat_t`, replacing the traditional `_Is_iterator<T>::value` pattern. The variable template language mechanism introduced in C++14 enabled the STL to replace `::value` trait classes with `_v`-suffixed variables, paving the way for C++17 standardized `_v` traits like `is_same_v`

## III. Notes

Variable templates support both full and partial specialization (unlike function templates, which only support full specialization). Don't overuse.

## IV. Exercise Code

### Exercise Topics

- 0 - [Basic Variable Templates — pi constant and compile-time config](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/02-variable-templates-0.cpp)
- 1 - [_v Type Traits — implementing is_void_v and is_same_v](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/02-variable-templates-1.cpp)

### Auto-Checker Command

```
d2x checker variable-templates
```

## V. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
