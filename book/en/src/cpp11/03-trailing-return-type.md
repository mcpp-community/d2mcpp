<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/03-trailing-return-type.html
[English]: ./03-trailing-return-type.html

# Trailing Return Type

The trailing return type is a new function declaration syntax introduced in C++11: `auto func(...) -> ReturnType`. It moves the return type from before the function name to after the parameter list. This solves a problem that the traditional syntax simply cannot express - "return type that depends on parameters" - and also provides the unified syntax for explicitly specifying lambda return types.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/function) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/03-trailing-return-type.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/03-trailing-return-type.cpp) |  |


**Why was it introduced?**

- In the traditional syntax, the return type is written before the function name, at which point **the parameters are not yet in scope**, so they cannot be used to deduce the return type
- Template programming often needs "return type = type of some expression involving the parameters", and without this syntax, deducing such a return type with decltype is essentially impossible to write
- It unifies function-signature shape: lambdas, ordinary functions, and template functions can all use the same `auto ... -> T` form

**How does it differ from the traditional return-type syntax?**

- Traditional form: `ReturnType func(Args...)`, with the return type at the front
- Trailing form: `auto func(Args...) -> ReturnType`, using auto as a placeholder and putting the actual return type after `->`
- The two are equivalent in most cases, but only the trailing form can **reference parameter names** after `->` - this is its truly irreplaceable capability

## I. Basic Usage and Scenarios

### Basic Syntax - auto + ->

Move the return type from in front of the function name to after the parameter list, joined by `->`. Use `auto` as a placeholder before the function name to indicate "the return type is given later".

```cpp
// Traditional form
int add(double a, int b) {
    return a + b;
}

// Trailing-return-type form - equivalent
auto add(double a, int b) -> int {
    return a + b;
}
```

The compiled output of these two forms is identical. For an ordinary function, the trailing form is purely a **stylistic choice** and adds no new capability.

### Combining decltype to Deduce a Parameter-Dependent Return Type

The real power of trailing return types shows up in templates. Suppose we want a generic add that supports int, double, Point, and any addable types. The return value should be the type of `a + b`, but the concrete type depends on T1 and T2.

The traditional form **cannot be written** here, because at the point where the return type appears, a and b are not yet in scope.

```cpp
// Cannot be written - a and b are not declared yet here
decltype(a + b) add(T1 a, T2 b);
```

The trailing syntax moves the return type after the parameters, where a and b are already in scope, so decltype can refer to them directly.

```cpp
template<typename T1, typename T2>
auto add(const T1 &a, const T2 &b) -> decltype(a + b) {
    return a + b;
}

add(1, 2);     // returns int
add(1.1, 2);   // returns double
add(1, 2.1);   // returns double
```

This is the **single irreplaceable use case** of the trailing return type: letting the return-type expression refer to parameter names.

> Since C++14, an ordinary function can simply be written as `auto add(...)` and the compiler will deduce the return type from the return statement, so `-> decltype(a + b)` is no longer required in most cases. But in C++11, the trailing form is still mandatory.

### Explicit Return Type for Lambdas

A lambda has no name, and therefore no choice of "where to put the return type" - it can only use the trailing syntax from the start.

```cpp
auto add = [](double a, double b) -> int {
    return a + b;  // explicitly truncated to int
};

add(1.1, 2.1); // 3, not 3.2
```

Without `-> int`, the lambda would deduce its return type as double; once explicitly annotated, the return value is converted to the specified type. This is the standard way to control a lambda's return type.

### Nested / Member Types as Return Types

When the return type is a class's nested type, the traditional form needs a fully qualified `typename Class::Inner` up front, with extra `typename` inside templates - long and verbose.

```cpp
template<typename T>
typename std::vector<T>::iterator find_first(std::vector<T> &v, T x);
```

The trailing form lets the function name appear first and writes the return type after `->`. In some class-member-function definitions, this also avoids repeating the class name prefix.

```cpp
struct Box {
    struct Inner { /* ... */ };

    auto make() -> Inner;  // return type is just Inner
};

auto Box::make() -> Inner {  // already inside the scope of Box at this point
    return Inner{};
}
```

## II. Important Notes

### auto in the Trailing Form Is Only a Placeholder

The auto in the trailing form is **not type deduction** - it is just a syntactic placeholder, with the actual type given after `->`. This is different from C++14's `auto func() { return ...; }`, where the compiler genuinely deduces the type.

```cpp
auto add(double a, int b) -> int {  // C++11: return type is given explicitly by -> int
    return a + b;
}

auto add(double a, int b) {         // C++14: return type is deduced from the return statement
    return a + b;
}
```

In C++11, writing `auto func()` without `->` is a compile error.

### When to Use Trailing vs. Traditional

Don't blindly convert every function to the trailing form. Some rules of thumb:

- Return type **depends on parameters** (e.g. decltype(a + b)) -> trailing is mandatory
- Lambda needs an explicit return type -> trailing is mandatory
- Member function returning a nested type, where you'd like to skip the `Class::` prefix -> trailing is cleaner
- Ordinary function with a simple return type (int / void / std::string) -> the traditional form reads better; no need to switch

### Not Every "auto func" Form Is a Trailing Return Type

`auto func() -> int` (C++11 trailing) and `auto func() { return 1; }` (C++14 return-type deduction) both start with `auto`, but their semantics are completely different.

- The former: auto is a placeholder, the real type is given by `->`, and the compiler **does not need to see the function body**
- The latter: the compiler must see the return statement to deduce the return type, so the declaration and definition cannot be cleanly separated (a header containing only the declaration cannot expose the return type)

When writing functions that need to be declared in a header and defined in a source file, this difference directly affects whether the code compiles at all.

### Return-Type Rules Still Apply

The trailing return type only **changes the position** - the rules of the type itself are unchanged:

- Cannot return a function or an array directly (need a function pointer / array pointer)
- References / const qualifications deduced by decltype are preserved
- When a derived class overrides a virtual function, the return type still has to satisfy the covariant-return rule

```cpp
int arr[3];
// auto func() -> int[3];  // error: cannot return an array
auto func() -> int(*)[3];   // ok: returns a pointer to an array
```

## III. Practice Code

### Practice Topics

- 0 - [Familiarize with trailing return type - ordinary function / template / lambda](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/03-trailing-return-type.cpp)

### Practice Code Auto-detection Command

```
d2x checker trailing-return-type
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
