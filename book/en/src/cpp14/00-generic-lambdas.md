<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp14/00-generic-lambdas.html
[English]: ./00-generic-lambdas.html

# Generic Lambdas

C++14 allows lambda parameters to use `auto`, turning the lambda's `operator()` into an implicit function template — a single lambda can accept arguments of different types

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-lambda](https://en.cppreference.com/w/cpp/language/lambda) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp14/00-generic-lambdas.md) | [Video Explanation]() | [Exercise Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp14/00-generic-lambdas-0.cpp) |  |


**Why introduced?**

- In C++11, lambda parameter types must be explicitly specified — the same lambda cannot be reused for different argument types because `operator()` is a plain member function, not a template
- Many lambdas express type-independent logic (e.g. comparing two values), but C++11 required writing separate lambdas for each concrete type (e.g. `[](int a, int b) { return a < b; }` for int, `[](double a, double b) { return a < b; }` for double)
- C++14 allows `auto` in lambda parameters; the compiler generates an implicit template for `operator()`, essentially bringing function templates into the lambda world

**How does it work?**

The compiler expands a generic lambda into a functor class with a **templated `operator()`**. For example, `[](auto a, auto b) { return a + b; }` is internally equivalent to:

```cpp
struct __lambda {
    template <typename T1, typename T2>
    auto operator()(T1 a, T2 b) const {
        return a + b;
    }
};
```

## I. Basic Usage and Scenarios

### Simple Generic Lambda

> Declare lambda parameters with auto; the compiler generates operator() instances based on the argument types at the call site

```cpp
auto identity = [](auto x) {
    return x;
};

int    i = identity(42);       // x deduced as int
double d = identity(3.14);     // x deduced as double
```

### Multi-Parameter Generic Lambda

```cpp
auto add = [](auto a, auto b) {
    return a + b;
};

add(1, 2);           // int + int
add(1.5, 2.5);       // double + double
add(std::string("hello "), std::string("world")); // string + string
```

Each parameter's type is deduced independently; `T1` and `T2` can differ:

```cpp
auto multiply = [](auto a, auto b) {
    return a * b;
};

multiply(2, 3.5);    // int * double → double
```

### Generic Lambda + STL Algorithms

The most common use case — avoid writing identical logic for every container element type:

```cpp
std::vector<int> v1 = {5, 1, 4, 2, 8};
std::vector<double> v2 = {3.1, 2.7, 8.5, 1.9};

// C++11: write separate lambdas for int and double
std::sort(v1.begin(), v1.end(), [](int a, int b) { return a > b; });
std::sort(v2.begin(), v2.end(), [](double a, double b) { return a > b; });

// C++14: a single generic lambda handles both
auto gt = [](auto a, auto b) { return a > b; };
std::sort(v1.begin(), v1.end(), gt);
std::sort(v2.begin(), v2.end(), gt);
```

### Generic Lambda with Captures

Captured variables keep their concrete types; only parameters use `auto`:

```cpp
int threshold = 10;
auto above = [threshold](auto x) {
    return x > threshold;  // threshold is int, x is generic
};

above(20);   // x = int
above(3.5);  // x = double
```

### Generic Lambda Returning Lambda

A generic lambda can return a new lambda, creating a function factory:

```cpp
auto make_adder = [](auto n) {
    return [n](auto x) { return x + n; };  // C++14 supports this
};

auto add5 = make_adder(5);
add5(10);   // 15
add5(3.14); // 8.14
```

## II. Real-World Case — Transparent Functors and Generic Lambdas in the STL

> C++14 introduced transparent operator functors (e.g. `std::less<>` / `std::greater<>`) alongside generic lambdas — both motivated by the same goal: eliminating the need to hard-code concrete types. The examples below cite the vendored [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) (source: [`msvc-stl/stl/inc/xutility`](https://github.com/mcpp-community/d2mcpp/blob/main/msvc-stl/stl/inc/xutility#L902-L912)); `_NODISCARD` / `constexpr` are internal annotations and can be ignored while reading

### std::greater Transparent Specialization — A Comparator That "Sees" Any Type

In C++11, `std::greater<T>` locked in the template parameter `T` — `std::greater<int>` could only compare `int`. C++14 added `std::greater<>` (equivalent to `std::greater<void>`), where `operator()` is itself a template accepting any comparable types:

```cpp
// MSVC STL · msvc-stl/stl/inc/xutility (abridged)
template <>
struct greater<void> {
    template <class _Ty1, class _Ty2>
    _NODISCARD constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
        noexcept(...) -> decltype(static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right)) {
        return static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right);
    }

    using is_transparent = int;
};
```

The `operator()` is a template member function — exactly the same underlying mechanism as a generic lambda. The `is_transparent` tag signals to associative containers like `std::set` and `std::map` that this comparator supports heterogeneous lookup, allowing a `std::string` to be used to search a `std::set<std::string>` without constructing a temporary object

### Transparent Comparators and Generic Lambdas — Two Sides of the Same Coin

Transparent functors and generic lambdas are interchangeable for the same use case:

```cpp
std::vector<int> v = {5, 1, 4, 2, 8};

// option 1: C++14 transparent comparator
std::sort(v.begin(), v.end(), std::greater<>());

// option 2: C++14 generic lambda
std::sort(v.begin(), v.end(), [](auto a, auto b) { return a > b; });
```

Both eliminate the C++11 redundancy of writing separate comparators for `int`, `double`, `string`, etc.

> Summary: C++14's transparent functors and generic lambdas are two expressions of the same idea — "parameterize the argument types". Transparent functors apply it to callable objects; generic lambdas apply it to lambdas. Understanding how the `is_transparent` tag connects both to STL algorithms shows why the standard library introduced these two features together in C++14

## III. Notes

### A Generic Lambda Is a Class with a Templated operator()

Each generic lambda expression produces a distinct closure type. Even two identical-looking generic lambdas have different types — the same rule as regular lambdas, but the `operator()` is a template, so the same type can accept different argument types

```cpp
auto f = [](auto x) { return x; };
auto g = [](auto x) { return x; };
// f and g have different types; cannot be assigned to each other
```

### Generic Parameters and Perfect Forwarding

Generic lambda parameter deduction strips references and const by default. Use `auto&&` with `std::forward` to preserve them:

```cpp
auto forwarder = [](auto&& x) -> decltype(auto) {
    return std::forward<decltype(x)>(x);
};
```

This pattern is common with generic lambdas and is a typical use case for `decltype(auto)` (another C++14 feature)

### Generic Lambdas Can Be Variadic

C++14 generic lambdas support parameter packs — `[](auto... xs)` accepts any number (and types) of arguments. The compiler generates a templated `operator()` with a parameter pack. C++20 later added explicit template parameter lists for lambdas (`[]<typename... Ts>(Ts... xs)`), but variadic generic lambdas were already usable in C++14

## IV. Exercise Code

### Exercise Code Topics

- 0 - [Basic Generic Lambda — auto parameters and type deduction](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/00-generic-lambdas-0.cpp)
- 1 - [Generic Lambda with STL Algorithms — sort, find, factory function](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/00-generic-lambdas-1.cpp)

### Exercise Auto-Checker Command

<details>
<summary>Don't have d2x? Click for setup</summary>

```bash
# 1. Install xlings (Linux / macOS)
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash
# Windows PowerShell:
# irm https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.ps1 | iex

# 2. Install d2x and get the tutorial
xlings install d2x -y
d2x install d2mcpp

# 3. Enter the project directory & run the checker
cd d2mcpp
```

</details>

```
d2x checker generic-lambdas
```

## V. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
