<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./17-pod.md
[English]: ../en/cpp11/17-pod.md

# POD (Plain Old Data)

In C, we usually call a struct that contains only simple data and can be safely copied with `memcpy` a POD (Plain Old Data).  
In C++, there is a corresponding category of **POD types** whose memory layout is compatible with C and can be passed to C interfaces directly as raw binary blocks.

| Book | Video | Code | Forum |
| --- | --- | --- | --- |
| [cppreference-PODType](https://cppreference.com/w/cpp/named_req/PODType.html) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/17-pod-type.md) | [Video Explanation]() | [Exercise Code]() | [Discussion Forum](https://forum.d2learn.org/category/20) |

> Note: since C++20, the “PODType” notion in the standard has been marked **deprecated**.  
> The standard library now prefers more fine-grained categories such as `TrivialType`, `StandardLayoutType`, and `ScalarType` to describe related requirements.

**Why introduce POD**

- C has many structs that contain only simple data and can be copied byte-by-byte, and C++ needs to remain compatible with such usage;
- It makes it convenient to use C++ types with C libraries and system calls, passing data by raw binary layout;
- It serves as a coarse-grained type category in early standards for “low-level optimization” and “ABI compatibility”.

**Relationship with other type categories**

- All **scalar types (`ScalarType`)** are POD, for example:
  - built-in arithmetic types: `int`, `double`, `char`, etc.;
  - enumerations `enum`;
  - various pointer types.
- For class types, the standard introduces:
  - **Trivial type (`TrivialType`)**: all special member functions (constructors, copy/move, destructor) are trivial (compiler-generated or `=default`, no virtual functions involved, etc.);
  - **Standard-layout type (`StandardLayoutType`)**: the object layout rules are simple and predictable (e.g. single inheritance, consistent access control).
- A class is a **POD class** if and only if:
  - it is a **trivial type**, and
  - it is a **standard-layout type**.

For example:
```cpp
struct A {
  int x;
  double y;
};              // POD: only built-in members, no user-defined special members

struct B {
  A a;
  int z;
};              // still POD: all members are POD types

struct C {
  virtual void foo();
  int x;
};              // not POD: has a virtual function, breaks trivial + standard-layout

struct D {
  int x;
private:
  int y;
};              // not POD: mixed public/private members, breaks standard layout
```

In practice, you can roughly think of a class as POD if it **only contains POD members, has no user-defined special members, no virtual functions/virtual inheritance, and a simple, consistent inheritance/access pattern**.

## I. Basic usage and typical scenarios

**Interacting with C interfaces**

> Use a POD struct to describe the binary layout required by a C interface so that it can be read/written as raw bytes.

```cpp
struct Packet {
  std::uint32_t len;
  std::uint16_t type;
  std::uint16_t flags;
};  // typical POD struct

int main() {
  Packet p{};
  // assume fd is an opened file or socket
  read(fd, &p, sizeof(p));
  write(fd, &p, sizeof(p));
}
```

**Simple memory snapshots**

> A POD type can be treated as a “byte array” for copying, which is usually safe on the same platform and with the same ABI/compile settings.

```cpp
struct Point {
  float x;
  float y;
};  // POD

void copy_points(const Point* src, Point* dst, std::size_t n) {
  std::memcpy(dst, src, n * sizeof(Point));  // byte-wise copy
}
```

**Working with type traits**

> Older code often uses `std::is_pod` to constrain template parameters; modern C++ prefers more fine-grained traits.

```cpp
template <typename T>
void pod_only_copy(const T& src, T& dst) {
  static_assert(std::is_pod<T>::value, "T must be POD");
  std::memcpy(&dst, &src, sizeof(T));
}
```

In new code, it is better to express constraints based on actual needs, for example:
- `std::is_trivially_copyable<T>` (trivially copyable)
- `std::is_standard_layout<T>` (standard layout)
- `std::is_scalar<T>` (scalar type)

## II. Notes

**POD does not guarantee cross-platform / cross-build binary compatibility**

- Different platforms, compilers, or compile options may result in different alignment and padding;
- If you persist raw POD bytes (e.g. files or network) and then `reinterpret_cast` them in another environment, differences in endianness, alignment, etc. can easily cause issues.

**The POD notion is deprecated in C++20**

- Since C++20, the standard marks “PODType” as **deprecated**;
- When adding or updating interfaces, prefer explicit, precise checks instead:
  - `std::is_trivial<T>` / `std::is_trivially_copyable<T>`
  - `std::is_standard_layout<T>`
  - `std::is_scalar<T>`

**Over‑focusing on “everything must be POD” hurts design flexibility**

- Many modern C++ types (`std::string`, `std::vector`, etc.) are not POD, but provide safer and more expressive abstractions;
- Usually only low-level modules that interact with C, perform binary serialization, or operate on raw memory need POD-like constraints; other code should favor modern C++ abstractions and safety features.

## III. Exercise code

### Exercise topics

- 0 – Use type traits to check POD / trivial / standard-layout (`17-pod-type-0.cpp`)
- 1 – Simulate byte-wise copying of a POD struct and observe the behavior (`17-pod-type-1.cpp`)
- 2 – Adapt C++ message types to C interface using POD headers (`17-pod-type-2.cpp`)

### Auto-check command

```bash
d2x checker pod-type
```

## IV. Other resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)