<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/15-variadic-templates.html
[English]: ./15-variadic-templates.html

# Variadic Templates

Variadic templates are a core template feature introduced in C++11. They allow function templates and class templates to take **any number of arguments of any type**, giving C++ the first type-safe and compile-time-checkable way to write `printf`-style multi-argument interfaces.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/parameter_pack) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/15-variadic-templates.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/15-variadic-templates-0.cpp) |  |


**Why was it introduced?**

- Before C++11, handling an arbitrary number of arguments forced you to either rely on C-style variadic macros (`...` / `__VA_ARGS__`) or hand-roll many overloads / macro-generated templates — neither was type-safe or maintainable
- The standard library needed a uniform mechanism to implement variadic components such as `make_shared`, `tuple`, and `function`
- Combined with rvalue references and perfect forwarding, variadic templates make truly "generic, zero-overhead" forwarding interfaces possible

**How does it differ from C-style variadics or hand-coded template overloads?**

- Variadic macros (`__VA_ARGS__`) are pure text substitution: no type information and no way to iterate the arguments at runtime — one wrong format specifier and it crashes
- Hand-coded template overloads (a separate version for 1, 2, 3, ..., N arguments) are extremely repetitive, hard to extend, and usually need macros to bulk-generate them
- Variadic templates expand parameter packs at compile time, fully preserving each argument's **type / value category / cv-qualification**, and integrate with `std::forward` for perfect forwarding

## I. Basic Usage and Scenarios

### Historical Context - Pre-C++11 Approaches

Before C++11, "any-number-of-arguments" was handled with **variadic macros** or **template overloads + macro generation**. Both have obvious shortcomings — the comparison below uses an arbitrary-argument output function as the running example.

**Variadic Macros**

Inherited from C: `...` declares the variadic part of the macro and `__VA_ARGS__` accesses the actual arguments.

```cpp
#define LOG(fmt, ...)  printf(fmt, __VA_ARGS__)

LOG("x = %d, y = %f\n", 10, 3.14); // expands to printf("x = %d, y = %f\n", 10, 3.14);
LOG("Hello");                       // expands to printf("Hello");
```

Easy to write but very limited, and hard to combine with modern C++:

- No type-safety checking
  - `LOG("%s", 42); // compiles, but crashes or prints garbage at runtime`
- Can't deal with references / move semantics, can't store the pack
- Can't iterate `__VA_ARGS__` and apply different operations per argument

**Template Overloads + Hard-Coding**

The most direct and most clumsy approach — write one overload per arity (1, 2, 3, ..., N). Extremely repetitive and hard to maintain.

**Macros Generating Templates**

A trick used heavily in `Boost.Preprocessor` — let the preprocessor generate `template<typename T1> void print(T1)`, `template<typename T1, typename T2> void print(T1, T2)`, ... overloads.

```cpp
#define TP_PARAM(n)    typename T##n
#define FN_PARAM(n)    T##n p##n
#define PRINT_BODY(n)  std::cout << p##n << " ";

#define REPEAT_TP_3     typename T1, typename T2, typename T3
#define REPEAT_FN_3     T1 p1, T2 p2, T3 p3
#define REPEAT_PRINT_3  PRINT_BODY(1) PRINT_BODY(2) PRINT_BODY(3)

#define DEFINE_LOG_FUNCTION(n) \
    template<REPEAT_TP_##n> \
    void log(REPEAT_FN_##n) { REPEAT_PRINT_##n std::cout << std::endl; }

DEFINE_LOG_FUNCTION(3)
// expands to:
// template <typename T1, typename T2, typename T3>
// void log(T1 p1, T2 p2, T3 p3) {
//     std::cout << p1 << " "; std::cout << p2 << " "; std::cout << p3 << " ";
//     std::cout << std::endl;
// }
```

It scales poorly, compiles slowly, and is painful to debug — exactly the problem variadic templates were meant to solve.

### Template Parameter Packs and Function Parameter Packs

C++11 uses `...` inside templates to denote a parameter pack. The classic `print` example:

```cpp
// recursion terminator
void print() { std::cout << std::endl; }

// pack-expanding template
template<typename T, typename... Args>
void print(T first, Args... args) {
    std::cout << first << " ";
    print(args...); // recursive call, peel off one argument each time
}

// print(1, "Hello", 3.14, 'A'); // works perfectly, type-safe
```

`...` shows up in **three positions** with three different meanings:

1. `template<typename T, typename... Args>` — modifies `typename`, declaring a **template parameter pack** (variable types)
2. `void print(T first, Args... args)` — modifies `Args`, declaring a **function parameter pack** (variable parameter list)
3. `print(args...)` — modifies the parameter name, performing **pack expansion** at the call site

### Pack Expansion - Recursive Peel-Off

C++11 has no direct syntax to iterate a pack. The standard idiom is recursion: each instantiation peels off one argument, then forwards the rest. The terminator is usually **a same-name non-template function** (overload resolution prefers non-templates) or a **single-argument template specialization**.

```cpp
// terminator: single-argument version
template<typename T>
T sum(T x) { return x; }

// expansion: multi-argument version
template<typename T, typename... Args>
T sum(T first, Args... args) {
    return first + sum(args...);
}
```

### Combining with Perfect Forwarding - make_shared

Variadic templates really shine **combined with universal references and perfect forwarding**, allowing arbitrary arguments to flow through to a target constructor untouched. `std::make_shared` in the standard library is the textbook example.

```cpp
template <typename T, typename... Args>
std::shared_ptr<T> make_shared(Args&&... args) {
    T* ptr = new T(std::forward<Args>(args)...);
    return std::shared_ptr<T>(ptr);
}
```

`Args&&...` is a "pack-form universal reference", and `std::forward<Args>(args)...` expands so that each element of the pack gets its own `std::forward`, preserving every argument's lvalue/rvalue category.

### sizeof... and C++14's index_sequence

C++14 added no new variadic syntax, but `std::index_sequence` / `std::make_index_sequence` enable **non-recursive** pack expansion: generate `0, 1, ..., N-1` at compile time, then expand the pack in a single template instantiation.

```cpp
template <typename T, std::size_t... Is>
void print_impl(T&& t, std::index_sequence<Is...>) {
    // expand the pack via comma expression + braced init-list, print one by one
    using expander = int[];
    (void)expander{ 0,
        ((std::cout << "Arg " << Is << ": "
                    << std::get<Is>(std::forward<T>(t)) << '\n'), 0)... };
}

template <typename... Args>
void print_args(Args&&... args) {
    auto t = std::make_tuple(std::forward<Args>(args)...);
    print_impl(t, std::make_index_sequence<sizeof...(Args)>{});
}
```

`sizeof...(Args)` returns the **count** of elements in the pack and is essentially required equipment when writing variadic templates.

### C++17 Fold Expressions - Replacing Recursion

C++17 introduced **fold expressions**, swapping recursive expansion for a much more direct syntax: apply a binary operator across the whole pack in a single expression, eliminating most boilerplate. There are four shapes:

- Unary left fold: `(... op pack)` — `((p1 op p2) op p3) op ...`
- Unary right fold: `(pack op ...)` — `p1 op (p2 op (p3 op ...))`
- Binary left fold: `(init op ... op pack)` — adds an initial value
- Binary right fold: `(pack op ... op init)`

```cpp
// unary right fold - sum
template <typename... Args>
auto sum(Args... args) { return (args + ...); }

// unary left fold - subtraction (mind evaluation order)
template <typename... Args>
auto sub(Args... args) { return (... - args); }

// binary left fold - subtraction with init: (((init - a1) - a2) - ... - aN)
template<typename T, typename... Args>
auto sub_with_init_left(T init, Args... args) { return (init - ... - args); }
```

Combined with `if constexpr`, you can branch on the pack at compile time and skip the dedicated empty terminator entirely.

```cpp
template<typename T, typename... Args>
void process_args(T first_arg, Args... rest_args) {
    process_value(first_arg);
    if constexpr (sizeof...(rest_args) > 0) { // compile-time check
        process_args(rest_args...);
    }
}
```

## II. Important Notes

### The Terminator Must Be Visible Beforehand

C++11-style recursive expansion relies on overload resolution to choose between "keep recursing" and "stop". The terminator (zero-argument or single-argument version) **must be visible before the recursive template**, otherwise you'll hit a no-matching-overload compile error.

### Template Recursion Depth Is Bounded

Every peeled argument is one more template instantiation. Compilers default to a depth limit around 1024 (raisable via `-ftemplate-depth=N`). For deeply nested or very wide expansions, prefer `index_sequence` or C++17 fold expressions — both are flatter and faster.

### Perfect-Forwarding a Pack Has a Fixed Spelling

A pack-form universal reference must be written `Args&&... args`, and forwarding must be written `std::forward<Args>(args)...` — the entire `forward<Args>(args)` is the pattern, and the trailing `...` expands once per element. Writing `std::forward<Args...>(args...)` is a common bug.

### A Pack Cannot Be "Saved" Directly

A parameter pack is not a first-class value — you can't assign it to a variable for later use. The standard workaround is to pack it into a `std::tuple` and consume it later via `std::index_sequence`.

```cpp
template <typename... Args>
auto save(Args&&... args) {
    return std::make_tuple(std::forward<Args>(args)...); // pack into a tuple
}
```

### Prefer C++17 Fold Expressions / Standard Library Helpers

If your project allows C++17, new code should prefer fold expressions + `if constexpr` over the "terminator + recursion" boilerplate. C++11-style recursive expansion is mostly relevant for **maintaining legacy code** or **projects locked to C++11**.

## III. Practice Code

### Practice Topics

- 0 - [Variadic templates basics - recursive print](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/15-variadic-templates-0.cpp)
- 1 - [Variadic template sum](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/15-variadic-templates-1.cpp)

### Practice Code Auto-detection Command

```
d2x checker variadic-templates
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
