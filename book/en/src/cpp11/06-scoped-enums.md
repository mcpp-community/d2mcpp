<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/06-scoped-enums.html
[English]: ./06-scoped-enums.html

# Scoped Enums

Scoped enums (`enum class` / `enum struct`) are strongly-typed enumerations introduced in C++11. They address several long-standing problems with traditional `enum`: enumerator names leaking into the enclosing scope, implicit conversion to `int`, and an underlying type that the programmer cannot control — turning enum values into a truly independent, type-safe set of discrete constants.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/enum) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/06-scoped-enums.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/06-scoped-enums-0.cpp) |  |


**Why was it introduced?**

- Traditional enum values leak into the enclosing scope, easily clashing with other names
- Traditional enums implicitly convert to int, leading to unsafe arithmetic and comparisons
- Traditional enums cannot specify an explicit underlying type, leaving the size unpredictable across platforms and compilers

**How does it differ from a traditional enum?**

- enum class enumerators do not leak into the enclosing scope; they must be accessed via `EnumName::Value`, and the same enumerator name can be reused across different enums
- enum class does not implicitly convert to an integral type; you must use `static_cast` whenever you need a number, and values from different enum classes cannot be compared
- enum class lets you specify an explicit underlying type (e.g. `: uint8_t`), giving precise control over memory layout, and also supports **forward declaration**

## I. Basic Usage and Scenarios

### Basic Syntax of enum class

`enum class` is the keyword combination for scoped enums (`enum struct` is equivalent). Just put `class` after the traditional `enum` keyword.

```cpp
enum class Color {
    RED,
    GREEN,
    BLUE,
    ORANGE
};

enum class Fruit {
    Apple,
    Banana,
    ORANGE // It is fine to share the name with Color::ORANGE — they live in different scopes
};
```

If these were traditional enums in the same scope, the duplicated `ORANGE` would cause a compile error.

### Explicit Scoping - Access via EnumName::Value

Enumerators of a scoped enum are not exposed to the enclosing scope, so accessing them always requires the enum name as a prefix.

```cpp
Color color = Color::ORANGE; // ok
Fruit fruit = Fruit::ORANGE; // ok — distinct from Color::ORANGE

// Color c = ORANGE; // error: ORANGE does not exist in the current scope
```

This forced prefix makes it immediately clear which enum a constant belongs to when reading the code, and completely eliminates symbol clashes.

### No Implicit int Conversion - Safer Comparisons and Arithmetic

A traditional enum implicitly decays to int, so a "color == fruit" comparison is silently accepted:

```cpp
enum Color { RED, GREEN, BLUE };
enum Fruit { Apple, Banana };

Color c = RED;
if (c == Apple) { /* compiles! Effectively 0 == 0, always true */ }
```

A scoped enum rejects such mistakes at compile time:

```cpp
enum class Color { RED, GREEN, BLUE };
enum class Fruit { Apple, Banana };

Color c = Color::RED;
// if (c == Fruit::Apple) { } // error: cannot compare different enum types
// int n = c;                 // error: no implicit conversion to int
int n = static_cast<int>(c);  // ok: must be an explicit cast
```

`==` / `!=` between two values of the *same* `enum class` is fine, but comparisons across different enums or against integers are rejected.

### Explicit Underlying Type - enum class X : uint8_t { ... }

The default underlying type of a scoped enum is `int`. You can specify a different one with `: type`, giving precise control over memory footprint.

```cpp
enum class Color {           // default underlying type is int
    RED, GREEN, BLUE
};

enum class Color8Bit : int8_t {   // explicitly int8_t
    RED, GREEN, BLUE, ORANGE
};

static_assert(sizeof(Color)     == sizeof(int),    "");
static_assert(sizeof(Color8Bit) == sizeof(int8_t), "");
```

Enumerator values can also be specified explicitly; any unspecified ones simply continue from the previous value + 1.

```cpp
enum class ErrorCode : int {
    OK      = 0,
    ERROR_1,        // 1
    ERROR_2 = -2,
    ERROR_3 = 3     // explicitly 3
};

static_cast<int>(ErrorCode::ERROR_3); // 3
```

This is invaluable for protocols, network packets, embedded registers, and anywhere memory layout matters.

### Forward Declaration Support

Because the underlying type of a scoped enum is fixed at declaration time (defaulting to `int` or explicitly given), you can declare it without listing the enumerators — a forward declaration.

```cpp
// header
enum class Status : uint8_t;       // forward declaration ok

void handle(Status s);             // usable in interfaces immediately

// .cpp
enum class Status : uint8_t {
    Ok, Pending, Failed
};
```

A traditional enum can't be forward-declared this way because its underlying type is inferred from the range of its enumerators (unless you also pin the underlying type explicitly, which is itself a C++11 extension).

## II. Important Notes

### When You Genuinely Need a Number, Use static_cast

A scoped enum does not auto-convert to int, so anytime you feed an enum value to an array index, a serializer, a logger, or an integer-only API, an explicit cast is required.

```cpp
enum class Color { RED, GREEN, BLUE };

Color c = Color::GREEN;

// int idx = c;                       // error
int idx = static_cast<int>(c);        // ok

std::cout << static_cast<int>(c);     // ok: otherwise << has no matching overload
```

Going the other way — building an enum value from an integer — also requires a cast: `Color c = static_cast<Color>(1);`. This deliberate friction nudges you to confirm the conversion is intentional and safe.

### Scoped Enums Don't Work as Bitmasks Out of the Box - Cast or Overload operator|

Traditional enums are often used as bit flags because `FLAG_A | FLAG_B` works directly. With scoped enums, the lack of implicit int conversion means this no longer compiles.

```cpp
enum class Perm : uint32_t {
    Read  = 1 << 0,
    Write = 1 << 1,
    Exec  = 1 << 2
};

// auto p = Perm::Read | Perm::Write; // error: no operator|
```

Two common workarounds:

```cpp
// Option 1: cast at the call site
auto p = static_cast<Perm>(
    static_cast<uint32_t>(Perm::Read) |
    static_cast<uint32_t>(Perm::Write)
);

// Option 2: overload operator| for this enum
constexpr Perm operator|(Perm a, Perm b) {
    return static_cast<Perm>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
    );
}
```

After Option 2 you can write `Perm::Read | Perm::Write` naturally while keeping type safety.

### Migrating Legacy Traditional Enums

Existing codebases tend to be full of traditional enums; you don't need to rewrite them all in one go. A pragmatic plan:

- Default new code to `enum class`
- When touching old code, prioritize "names that are obviously prone to clash" and "comparisons that rely on implicit int conversion"
- For memory-layout-sensitive enums, also pin the underlying type with `: uint8_t` / `: uint16_t` etc. while you're there
- For bit-flag use cases, prefer casting or overloading `operator|` / `operator&`; don't fall back to traditional enums

## III. Practice Code

### Practice Topics

- 0 - [Pitfalls of traditional enum - name clashes and implicit comparison](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/06-scoped-enums-0.cpp)
- 1 - [Basic usage of scoped enum - scoping / type safety / underlying type](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/06-scoped-enums-1.cpp)

### Practice Code Auto-detection Command

```
d2x checker scoped-enums
d2x checker scoped-enums-1
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
