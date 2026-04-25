<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/07-constexpr.html
[English]: ./07-constexpr.html

# constexpr - Compile-Time Computation

constexpr is a keyword introduced in C++11 that lifts "results that would normally only be available at runtime" up to **compile time**, so the compiler has the concrete value before code generation, while still keeping the same code callable at runtime.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/constexpr) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/07-constexpr.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/07-constexpr-0.cpp) |  |


**Why was it introduced?**

- Move computations that can be done at compile time off the runtime hot path
- Give the compiler stronger invariant guarantees (array sizes, template non-type parameters, and other positions that strictly require compile-time constants can now be filled with the result of a function call)
- Pair naturally with template metaprogramming / static_assert / enum, explicitly stating "this value is a compile-time known quantity"

**How is constexpr different from const?**

- const: "I won't modify it" — the value may not be known until runtime; once initialized, it just can't be changed
- constexpr: "this value is fixed at compile time" — a stronger contract that the compiler must be able to evaluate
- Every constexpr variable is also const, but not every const can be used where `constexpr` is required (e.g. array dimensions, template non-type parameters)

## I. Basic Usage and Scenarios

### constexpr Variables — Must Be Initialized With a Compile-Time Constant

A constexpr variable's initializer must be evaluable at compile time, otherwise it's a hard compile error. The whole point is "compile-time constant".

```cpp
int n = 10;
const int a = n + 10;     // ok: a is const, but its value is determined at runtime
constexpr int b = 10 * 3; // ok: b is 30 at compile time
// constexpr int c = n;   // error: n is a runtime variable, can't initialize a constexpr
```

`const` only promises "no further modification", whereas `constexpr` additionally requires "computable right now".

### Compile-Time vs Runtime Constant — The Difference at Array Dimensions

In C++, array dimensions must be compile-time constants. At this position, neither a plain `int` nor a "const derived from a runtime variable" is reliable — only constexpr is guaranteed to work.

```cpp
int size1 = 10;
const int size2 = size1 + 10;
constexpr int size3 = 10 * 3;

int arr1[size3]; // ok: size3 is a compile-time constant
// int arr2[size1]; // error: size1 is a runtime variable
// int arr3[size2]; // depends on the compiler: size2 is derived from size1, not necessarily compile-time
```

In Exercise 0 you have to pick the one `sizex` that is reliably known at compile time inside `arr1[sizex]` — the answer is `size3`.

### constexpr Functions — Usable at Both Compile Time and Runtime

The defining property of a constexpr function is its **dual nature**: pass it compile-time constant arguments and it runs at compile time; pass it runtime variables and it falls back to a normal function call.

```cpp
constexpr int sum_for_1_to(int n) {
    return n == 1 ? 1 : n + sum_for_1_to(n - 1);
}

int main() {
    constexpr int s1 = sum_for_1_to(4); // computed to 10 at compile time
    int n = 5;
    int s2 = sum_for_1_to(n);           // computed at runtime
}
```

Note: marking a function constexpr does **not** force it to always run at compile time — whether it does depends on **where it is used** and **what arguments it receives**.

### Using constexpr Functions Where Compile-Time Constants Are Required

Array dimensions, template non-type parameters, `static_assert`, `case` labels — all require compile-time constants. Wrapping the calculation in a constexpr function lets you call it directly in any of these positions.

```cpp
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fact_10 = factorial(10);
int arr[factorial(5)];                  // array dimension: ok
static_assert(factorial(5) == 120, ""); // static_assert: ok
```

### Pairing With Templates for Compile-Time Computation

Template non-type parameters (`template <int N>`) also require compile-time constants. constexpr functions and constexpr variables both satisfy that.

```cpp
template <int N>
struct Sum {
    static constexpr int value = Sum<N - 1>::value + N;
};

template <>
struct Sum<1> { static constexpr int value = 1; };

constexpr int sum_4 = Sum<4>::value; // 10 at compile time
```

Combining `factorial` and `Sum` lets you solve small problems entirely at compile time — for example Exercise 1 asks "what value of `value` makes `value! + (1+2+..+value) > 10000`?", with no runtime work needed.

```cpp
constexpr int value = 8;
constexpr int f = factorial(value);
constexpr int s = Sum<value>::value;
constexpr int ans = f + s;
static_assert(ans > 10000, "ans should be > 10000");
```

### C++11 Restrictions on constexpr Function Bodies

C++11's rules for constexpr function bodies are strict:

- The body is essentially limited to **a single return statement** (use the ternary `?:` for branching)
- No loops (use recursion instead)
- No local variable definitions, no mutation

```cpp
// ok: C++11-style constexpr function — single return + ternary + recursion
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}
```

C++14 relaxed these restrictions, allowing local variables, loops, and multiple statements — making constexpr functions look almost identical to ordinary ones. For this C++11 chapter, however, we stick to the "single return + recursion" pattern.

## II. Important Notes

### Passing a Runtime Argument to a constexpr Function Is Not an Error — It Just Runs at Runtime

constexpr is a **capability declaration**, not a **usage requirement**. The same constexpr function is forced to run at compile time when initializing a constexpr variable, and runs as an ordinary runtime function in normal assignments.

```cpp
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

int n = 5;
int a = factorial(n);           // ok: runtime call (n is a runtime variable)
constexpr int b = factorial(5); // ok: compile-time call
// constexpr int c = factorial(n); // error: constant context needed, but n isn't constant
```

### Functions Called Inside a constexpr Function Must Themselves Be constexpr

If a constexpr function internally calls a non-constexpr function, it can't be used in a compile-time context — the compiler will refuse to treat the whole expression as a constant expression.

```cpp
double pow(double base, int exp) {              // not constexpr
    return exp == 0 ? 1.0 : base * pow(base, exp - 1);
}

constexpr double mysin(double x) {
    return x - pow(x, 3) / 6.0; // error: pow is not constexpr
}
```

Once `pow` is changed to `constexpr double pow(...)`, `mysin` can really evaluate at compile time. The `mysin(30.0)` in Exercise 1 is exactly this "lookup-table by compiler" pattern — once fixed, the entire sin value is computed by the compiler and baked into the binary, with O(1) runtime cost.

### Don't Add constexpr Just for the Sake of It

Pushing computation to compile time also pushes error reporting and debugging information to compile time, and once the logic gets complex, compiler diagnostics can become intimidating. Rules of thumb:

- A computation that genuinely needs to appear in an array dimension / template argument / static_assert → use constexpr
- An ordinary utility function that could be constexpr but doesn't need to be → don't bother, keep the cost story simple

### constexpr Is Not the Same as inline or noexcept

But here's a detail that's easy to miss: **constexpr functions are implicitly inline**. This means including the same constexpr function definition across multiple translation units does not violate ODR — defining it directly in a header file is fine, no extra `inline` needed.

## III. Practice Code

### Practice Topics

- 0 - [Compile-time computation basics: constexpr vs const](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/07-constexpr-0.cpp)
- 1 - [Applied compile-time computation: factorial / Sum / mysin auto lookup table](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/07-constexpr-1.cpp)

### Practice Code Auto-detection Command

```
d2x checker constexpr
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
