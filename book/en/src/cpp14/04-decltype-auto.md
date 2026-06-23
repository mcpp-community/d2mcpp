<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp14/04-decltype-auto.html
[English]: ./04-decltype-auto.html

# decltype(auto)

C++14 introduces `decltype(auto)` as a return type, deducing using decltype rules to precisely preserve reference and cv-qualification

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-auto](https://en.cppreference.com/w/cpp/language/auto) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/04-decltype-auto.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/04-decltype-auto-0.cpp) |  |


**Why introduced?**

- `auto` return strips references and top-level const, causing unexpected copies in forwarding and getters
- `decltype(auto)` deduces using decltype rules, precisely preserving the value category and type qualifiers

**auto return vs decltype(auto)**

```cpp
int x = 42;

auto         f1() { return (x); }  // returns int
decltype(auto) f2() { return (x); }  // returns int&, preserves reference
```

## I. Basic Usage and Scenarios

### Preserving Reference Returns

```cpp
int global = 100;

decltype(auto) get_ref() {
    return (global);  // returns int&, modifiable externally
}

get_ref() = 200;  // modifies global
```

### Forwarding Pattern

```cpp
template <typename T>
decltype(auto) forwarder(T&& arg) {
    return std::forward<T>(arg);
}
```

### Container Element Access

```cpp
std::vector<int> v = {1, 2, 3};

decltype(auto) get_at(std::vector<int>& vec, size_t i) {
    return vec[i];  // returns int&
}
```

## II. Real-World Case — decltype(auto) Forwarding in the STL

> The MSVC STL uses `decltype(auto)` for forwarding call results while precisely preserving the return value category. The example below cites the vendored [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) (source: [`msvc-stl/stl/inc/xutility`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/xutility#L943-L947))

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (abridged)
template <class... _Args>
constexpr decltype(auto) operator()(_Args&&... _Vals) noexcept(...) {
    return _STD invoke(_Fn, _STD forward<_Args>(_Vals)...);
}
```

`std::invoke` may return an lvalue reference (e.g. `std::get<T&>(tuple)`) or an rvalue (e.g. move semantics). `decltype(auto)` ensures the original value category reaches the caller — no accidental copies from auto stripping, no loss of const qualification

## III. Notes

### Parentheses Trap

`decltype(auto)` treats `return x;` and `return (x);` differently:

```cpp
int x = 10;
decltype(auto) f1() { return  x; }  // int
decltype(auto) f2() { return (x); } // int& — watch out!
```

### decltype(auto) in Variable Declarations

```cpp
int x = 42;
decltype(auto) y = x;   // int
decltype(auto) z = (x);  // int& — same parentheses rule applies
```


## IV. Exercise Code

### Exercise Topics

- 0 - [decltype(auto) — Reference Preservation and Forwarding](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/04-decltype-auto-0.cpp)
- 1 - [decltype(auto) — Parentheses Trap and Variable Declarations](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/04-decltype-auto-1.cpp)

### Auto-Checker Command

```
d2x checker decltype-auto
```

## V. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
