<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/01-default-and-delete.html
[English]: ./01-default-and-delete.html

# Defaulted and Deleted Functions

`= default` and `= delete` are two **function-definition forms** introduced in C++11 that let the programmer state, at the source level, "I want the compiler to generate this special member with its default implementation" or "this function must not be called". They hand back to the designer the control over special members that previously could only be inferred from the compiler's implicit rules.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/function#Deleted_functions) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/01-default-and-delete.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-0.cpp) |  |


**Why were they introduced?**

- Before C++11, as soon as a class declared any user-defined constructor, the compiler stopped synthesizing the default constructor — and there was no explicit way to "ask for it back"
- There was no standard way to express "I deliberately don't want this special member — calling it should be a compile error". The old workaround was "declare the copy constructor private and don't define it", which produced cryptic errors that surfaced only at link time
- Different compilers' implicit rules for "when a special member is auto-generated or auto-deleted" are easy to misremember; explicit markers let intent live directly in the code

**What do = default and = delete mean?**

- `= default`: ask the compiler to generate this special member with its default implementation (default ctor / dtor / copy / move / [C++20] comparison operators, etc.) — equivalent to "I want the generated version; don't implicitly drop it just because I wrote some other member"
- `= delete`: explicitly forbid a function — any call to it, or any overload resolution that selects it, fails at **compile time** with a clear "use of deleted function" diagnostic

## I. Basic Usage and Scenarios

### Explicit default - Bring Back the Suppressed Default Constructor

The moment a class introduces any user-defined constructor, the compiler stops synthesizing the default constructor. The `B` below silently disallows `B b;`.

```cpp
struct B {
    B(int x) { std::cout << "B(int x)" << std::endl; }
};

B b;        // error: no default constructor
B b2(10);   // ok
```

Adding `= default` brings the default constructor back without affecting the user-defined one.

```cpp
struct B {
    B() = default;                                       // explicitly request the default ctor
    B(int x) { std::cout << "B(int x)" << std::endl; }   // user-defined ctor
};

B b;        // ok
B b2(10);   // ok
```

Similarly, in `C` below, having both a no-arg constructor and a one-arg constructor with a default value would create an **ambiguity** at `C c;`. Writing the no-arg version as `= default` and dropping the default value on the other makes the intent obvious.

```cpp
struct C {
    C() = default;
    C(int x) { std::cout << "C(int x): " << x << std::endl; }
};

C c1;      // calls C()
C c2(1);   // calls C(int)
```

### Explicit delete - Build a Non-Copyable Type

`std::unique_ptr`'s key semantics are "exclusive ownership -> non-copyable but movable". A simplified hand-written version is just: `= delete` the two copy operations and `= default` the two move operations.

```cpp
struct UniquePtr {
    void *dataPtr;
    UniquePtr() = default;

    UniquePtr(const UniquePtr&) = delete;             // forbid copy construction
    UniquePtr& operator=(const UniquePtr&) = delete;  // forbid copy assignment

    UniquePtr(UniquePtr&&) = default;                 // allow move construction
    UniquePtr& operator=(UniquePtr&&) = default;      // allow move assignment
};

UniquePtr a;
UniquePtr b = a;             // error: copy ctor is deleted
UniquePtr c = std::move(a);  // ok: move ctor
```

Type traits make it possible to verify these semantics at compile time.

```cpp
static_assert(std::is_copy_constructible<UniquePtr>::value == false, "");
static_assert(std::is_copy_assignable<UniquePtr>::value    == false, "");
static_assert(std::is_move_constructible<UniquePtr>::value == true,  "");
static_assert(std::is_move_assignable<UniquePtr>::value    == true,  "");
```

### Using = delete to "Mask" Specific Parameter Types in an Overload Set

`= delete` is not limited to special members — it works on any overload of any function. A common pattern is **blocking implicit conversions** by deleting the unwanted overload, so that calling with the wrong argument type fails at compile time.

```cpp
void func(int x) {
    std::cout << "x = " << x << std::endl;
}

// Explicitly delete the float overload, otherwise func(1.1f) would silently
// be converted to int.
void func(float) = delete;

func(1);       // ok: int overload
func(1.1f);    // error: call of deleted function
```

Without that deleted overload, `func(1.1f)` would silently undergo a narrowing `float -> int` conversion and lose `0.1`. With it removed from the overload set, the diagnostic is unambiguous: "use of deleted function 'void func(float)'".

### Where default / delete Apply

`= default` is valid only on the special member functions the compiler can generate:

- default constructor (no arguments)
- destructor
- copy constructor / copy assignment
- move constructor / move assignment
- (C++20) `<=>` and other defaulted comparison operators

`= delete`, on the other hand, has no such restriction — any function declaration (free function, member, template specialization, special member) can be deleted.

## II. Important Notes

### = default Does Not Imply "Trivial"

Writing `= default` just means "let the compiler generate it"; whether the generated version is trivial or noexcept depends on the bases and members. If a base's copy constructor is non-trivial, the derived class's `= default` copy constructor is also non-trivial.

```cpp
struct HasString {
    std::string s;        // string's copy ctor is not trivial
    HasString(const HasString&) = default;
};

static_assert(!std::is_trivially_copy_constructible<HasString>::value, "");
```

If your code relies on triviality (memcpy-style copying, placement in a union, etc.), don't conclude anything from `= default` alone — verify it with `std::is_trivially_*` traits.

### delete Works on Ordinary Functions Too

`= delete` is not exclusive to special members. Any function can be deleted — useful both for blocking specific overloads and for forbidding particular template specializations.

```cpp
template <typename T>
void only_int(T) = delete;   // forbid everything by default

template <>
void only_int<int>(int x) {  // only allow int
    std::cout << x << std::endl;
}

only_int(1);       // ok
only_int(1.0);     // error: call of deleted function
```

### Don't Make Deleted Members private

The pre-C++11 idiom for non-copyable types declared the copy ctor / copy assign as private and left them undefined. That style is obsolete — switch to `= delete` (in the public section), because:

- `= delete` reports the error during overload resolution with a clear diagnostic; private + undefined surfaces only as a link-time error
- Putting it in public guarantees every caller sees the same diagnostic; otherwise friends or in-class members would still resolve the call and run into a different error shape

### Rule of 0 / 3 / 5 - Designing Classes With default and delete

The real design value of `= default` / `= delete` shows up in combination with the **Rule of 0 / 3 / 5**:

- **Rule of 0**: the class manages no resources directly and relies entirely on members' RAII (e.g. `std::string`, `std::vector`, `std::unique_ptr`) -> declare none of the special members and let the compiler synthesize them
- **Rule of 3 (C++98)**: if you implement any of copy ctor, copy assign, or destructor, you typically need to implement the other two
- **Rule of 5 (C++11)**: with move semantics added, include move constructor and move assignment too — once you explicitly define / delete / default any one of these five, write all five out so that the compiler's implicit rules can't surprise you

## III. Practice Code

### Practice Topics

- 0 - [Explicitly control constructor generation behavior](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-0.cpp)
- 1 - [Implement a non-copyable but movable UniquePtr](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-1.cpp)
- 2 - [Use = delete to block a specific parameter-type overload](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-2.cpp)

### Practice Code Auto-detection Command

```
d2x checker default-and-delete
d2x checker default-and-delete-1
d2x checker default-and-delete-2
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
