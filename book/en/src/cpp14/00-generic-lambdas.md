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
- Many lambdas express type-independent logic (e.g. `[](auto a, auto b) { return a < b; }` works for `int`, `double`, `string`), but C++11 required writing a separate lambda for each type
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

## II. Notes

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

### Generic Lambdas Are Not Variadic

The parameter count is still fixed — `[](auto a, auto b)` accepts exactly two arguments. For variadic parameters, you still need variadic templates (C++20 later added support for `...` parameter packs in lambdas)

## III. Exercise Code

### Exercise Code Topics

- 0 - [Basic Generic Lambda — auto parameters and type deduction](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/00-generic-lambdas-0.cpp)
- 1 - [Generic Lambda with STL Algorithms — sort, find, factory function](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/en/cpp14/00-generic-lambdas-1.cpp)

### Exercise Auto-Checker Command

```
d2x checker generic-lambdas
```

## IV. Other

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/openxlings/xlings)
