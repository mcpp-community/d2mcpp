<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/04-rvalue-references.html
[English]: ./04-rvalue-references.html

# Rvalue References

Rvalue reference `T&&` is a new kind of reference introduced in C++11 that binds precisely to **rvalues / about-to-expire objects**. It lets the compiler distinguish "a temporary whose resources can be stolen" from "a named object that must be preserved" during overload resolution, and it is the syntactic foundation for move semantics and perfect forwarding.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/reference) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/04-rvalue-references.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/04-rvalue-references.cpp) |  |


**Why was it introduced?**

- Before C++11, the only way to bind a temporary was `const T&`, which gave a read-only view — there was no way to reuse its resources without copying
- There was no mechanism that could "recognize an rvalue" at the overload-resolution level, so the compiler could not separate "constructed from a temporary" from "constructed from a normal object"
- It provides the syntactic foundation for move semantics (`std::move`) and perfect forwarding (`std::forward`), making "stealing resources from an expiring object" something the language can express

**What's the difference between lvalues and rvalues?**

- **lvalue**: an expression with a name, persistent storage, and an addressable identity — for example a declared variable
- **rvalue**: usually a literal, a temporary, or a non-reference function return value — short-lived and not directly addressable
- Rule of thumb: **if it can appear on the left of `=` and you can take its address with `&`**, it's an lvalue; otherwise it's most likely an rvalue

## I. Basic Usage and Scenarios

### Telling lvalues and rvalues Apart

To classify an expression, ask whether it has "a name + a persistent identity".

```cpp
int a = 1;        // a is an lvalue
int b = a + 1;    // a + 1 is an rvalue (no name, just a temporary result)

&a;        // ok: an lvalue is addressable
// &(a + 1); // error: an rvalue is not addressable

int &lref  = a;       // ok: lvalue reference binds an lvalue
// int &lref2 = a + 1; // error: a plain lvalue reference cannot bind an rvalue
```

### Declaring and Binding an Rvalue Reference

`T&&` is the rvalue-reference syntax. It **only binds to rvalues**.

```cpp
int &&rref1 = 10;        // ok: a literal is an rvalue
int &&rref2 = a + 1;     // ok: a temporary computation is an rvalue

// int &&rref3 = a;      // error: an rvalue reference cannot bind an lvalue directly
```

Once bound, the temporary's lifetime is **extended** to the end of the reference variable's scope — same as the rule for `const T&` — except that an rvalue reference gives you a **mutable view**.

```cpp
struct Object {
    int data = 0;
};

const Object &cref = Object(); // lifetime extended, but read-only
// cref.data = 1;              // error: cannot mutate through a const reference

Object &&rref = Object();      // lifetime extended, and writable
rref.data = 1;                 // ok
```

This is exactly what the practice code is verifying: `objRef.data = 1;` must compile, while `&objRef` still points to the same temporary whose lifetime was extended.

### Distinguishing lvalues and rvalues in Overloads

Using an rvalue reference as a function parameter lets the compiler take two different overload paths for "passing an lvalue" vs "passing an rvalue".

```cpp
struct Object {
    Object() { std::cout << "Object()\n"; }
    Object(const Object&) { std::cout << "Object(const Object&)\n"; }
    Object(Object&&)      { std::cout << "Object(Object&&)\n"; }
};

void use(const Object&) { std::cout << "use lvalue\n"; }
void use(Object&&)      { std::cout << "use rvalue\n"; }

int main() {
    Object a;
    use(a);          // -> use lvalue   (a is an lvalue)
    use(Object());   // -> use rvalue   (a temporary is an rvalue)
}
```

The practice's `Object` defines both a copy constructor `Object(const Object&)` and a move constructor `Object(Object&&)` precisely so the prints tell you "which path was taken".

### Extending a Temporary's Lifetime

Here is the simplified scenario from the exercise: bind the prvalue `Object()` to a reference, and the temporary's destruction is deferred until the reference variable leaves scope.

```cpp
{
    Object &&objRef = Object(); // temporary's lifetime extended to here
    objRef.data = 1;            // mutate it through the rvalue reference
} // destructor runs here
```

Switching to `const Object &objRef = Object();` extends the lifetime in the same way, but the line `objRef.data = 1;` would no longer compile — that is the most direct difference between `const T&` and `T&&` in this scenario.

## II. Important Notes

### An Rvalue Reference Variable Itself Is an lvalue

In `int &&rref = 10;`, the **name** `rref` has identity and is addressable, so when used in an expression it is an **lvalue** — no longer an rvalue.

```cpp
void use(const Object&) { std::cout << "lvalue path\n"; }
void use(Object&&)      { std::cout << "rvalue path\n"; }

Object &&rref = Object();
use(rref); // -> lvalue path  (rref is an lvalue in expressions!)
```

If you want to pass it on as an rvalue again, you must explicitly cast with `std::move(rref)` — which is the entry point into the next chapter on move semantics.

### Overload Resolution Between const Reference and Rvalue Reference

When both `const T&` and `T&&` overloads exist, the compiler prefers the `T&&` version for an **rvalue argument**.

```cpp
void f(const Object&) { std::cout << "const &\n"; }
void f(Object&&)      { std::cout << "&&\n"; }

f(Object()); // -> &&     (rvalue prefers the rvalue reference)

Object a;
f(a);        // -> const & (lvalue matches the const lvalue reference)
```

This rule is what allows STL containers (such as `std::vector::push_back`) to take separate "copy" and "move" paths depending on the value category of the argument.

### Don't Confuse `T&&` With "Universal Reference"

In **template argument deduction**, `T&&` becomes a "universal / forwarding reference", which behaves differently from a plain rvalue reference — the same syntax in `template<typename T> void f(T&&)` accepts both lvalues and rvalues. That belongs to perfect forwarding and is covered later. For now, just remember: in **non-template contexts**, `T&&` is an rvalue reference and accepts only rvalues.

```cpp
void g(Object&& o);             // accepts rvalues only
template<typename T> void h(T&&); // universal reference, accepts both
```

### Rvalue References Are the Entry Point of Move Semantics, Not the Whole Story

This chapter focuses on the **value-category + reference-binding** layer. The part where rvalue references actually "steal resources" (move constructor / move assignment / `std::move`) is covered in ch05. That said, the move-constructor print `Object(Object&&)` in the practice already lets you observe end-to-end how "rvalue argument -> move path" works.

## III. Practice Code

### Practice Topics

- 0 - [Use an rvalue reference to extend a temporary's lifetime and mutate it](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/04-rvalue-references.cpp)

### Practice Code Auto-detection Command

```
d2x checker rvalue-references
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
