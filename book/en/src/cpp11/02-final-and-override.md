<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/02-final-and-override.html
[English]: ./02-final-and-override.html

# final and override - Explicit Control of Virtual Function Behavior

final and override are two **context-sensitive identifiers** introduced in C++11, used in virtual-function inheritance to explicitly express the intent of **overriding** and **sealing**, allowing the compiler to surface polymorphism mismatches at compile time that would otherwise only show up as runtime bugs.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-final](https://en.cppreference.com/w/cpp/language/final) / [cppreference-override](https://en.cppreference.com/w/cpp/language/override) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/02-final-and-override.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/02-final-and-override-0.cpp) |  |


**Why were they introduced?**

- Before C++11, whether a derived class actually overrode a base virtual function relied entirely on programmers checking signatures by hand — a single mismatched parameter would silently turn an override into a name-hiding declaration with no compiler warning
- There was no standard way to express the design intent that "this type, or this polymorphic chain, ends here"
- They make the design contract of virtual functions readable and verifiable

**What's the difference between the two?**

- override: applied after a derived-class member function, explicitly declaring "this function overrides a base-class virtual function", so the compiler can verify it
- final: applied after a virtual function means "this virtual function cannot be further overridden"; applied after a class means "this class cannot be inherited from"

## I. Basic Usage and Scenarios

### override - Explicitly Declare an Override

Without override, even a typo in the derived signature only becomes "a brand-new ordinary function" — the polymorphic behavior is silently lost.

```cpp
struct Base {
    virtual void func(int) { }
};

struct Derived : Base {
    void func(double) { } // intended to override, but the parameter type is wrong;
                          // this actually declares a new function
};
```

With override, the same mistake is rejected at compile time.

```cpp
struct Derived : Base {
    void func(double) override; // error: no matching virtual function in any base class
};
```

Only a base virtual function whose signature (return type + parameter list + cv-qualifiers + ref-qualifiers) matches exactly will satisfy override.

```cpp
struct Base {
    virtual void func(int);
};

struct Derived : Base {
    void func(int) override; // ok
};
```

### final - Forbid Further Overriding or Inheritance

final has two usages targeting different things.

**On a virtual function - cut off the polymorphic chain**

```cpp
struct A {
    virtual void func() final { }
};

struct B : A {
    void func() override; // error: A::func is final and cannot be overridden
};
```

**On a class - forbid inheritance**

```cpp
struct B final { };

struct C : B { }; // error: B is final and cannot be inherited from
```

### final + Pure Virtual - Non-Overridable Template Method (NVI)

Lock the outer interface with `virtual ... final`, and expose the customizable steps as pure virtual functions. The result is a stable interface where **the execution order cannot be changed but each step is customizable**. This is a concise expression of the **Non-Virtual Interface** idiom.

```cpp
struct AudioPlayer {
    virtual void play() final {  // subclasses cannot change the overall flow of play
        init_audio_params();
        play_audio();
    }
private:
    virtual void init_audio_params() = 0; // left for subclasses to customize
    virtual void play_audio() = 0;
};

struct WAVPlayer : AudioPlayer {
    void init_audio_params() override { /* ... */ }
    void play_audio() override { /* ... */ }
};

struct MP3Player : AudioPlayer {
    void init_audio_params() override { /* ... */ }
    void play_audio() override { /* ... */ }
};
```

Callers always use the unified `AudioPlayer::play()`; each format's player only needs to implement the two hooks. This structure is common when designing plugin-style or protocol-style interfaces.

### Context-Sensitive Identifiers

Neither override nor final is a reserved word or a keyword — they are **context-sensitive identifiers**. They only carry these meanings when they appear at specific positions in a virtual function declaration or a class declaration; in any other position they can still be used as variable names, type names, namespace names, etc.

```cpp
B override; // ok: here override is just an ordinary variable name
B final;    // ok: here final is just an ordinary variable name
```

This is a deliberate compromise in the C++ standard for backward compatibility: existing code that uses override or final as identifiers won't fail to compile after upgrading to C++11.

## II. Important Notes

### override Requires a Signature-Matching Base Virtual Function

Once override is added to a derived-class member function, the compiler requires **a virtual function with a matching signature** to exist in some base class — otherwise it's a compile error. This is the core value of override: lifting "override mismatch" silent bugs from runtime to compile time.

```cpp
struct A {
    virtual void func1() { }
    void func2() { } // note: not virtual
};

struct B : A {
    void func1() override; // ok
    void func2() override; // error: A::func2 is not virtual
};
```

### A final Class Is "Sealed" - Use With Care

A final class cannot be inherited from at all, not even to add a couple of helper methods. Marking a class final is essentially committing to "this type is, by design, a leaf node". Some rules of thumb:

- The type is explicitly not meant to be extended further (e.g. error types, framework-internal implementation classes, singletons) -> a good fit for final
- A general-purpose base class or a framework-provided extension point -> do not casually add final

### final Only Applies to Virtual Functions

An ordinary member function cannot be overridden in the first place, so adding final to it is meaningless and the compiler will reject it.

```cpp
struct A {
    void func() final; // error: final cannot be applied to a non-virtual function
};
```

### override and final Can Be Used Together

If a virtual function should both override the base-class version and forbid further overriding in derived classes, you can combine the two.

```cpp
struct B : A {
    void func() override final; // overrides A::func, and prevents C from overriding it again
};
```

## III. Practice Code

### Practice Topics

- 0 - [Familiarize with override](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/02-final-and-override-0.cpp)
- 1 - [Familiarize with final](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/02-final-and-override-1.cpp)
- 2 - [final + Template Method - AudioPlayer / WAV / MP3 / OGG](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/02-final-and-override-2.cpp)

### Practice Code Auto-detection Command

```
d2x checker final-and-override
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
