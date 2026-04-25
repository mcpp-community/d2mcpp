<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/05-move-semantics.html
[English]: ./05-move-semantics.html

# Move Semantics

Move semantics is a **resource ownership transfer** mechanism introduced in C++11 on top of rvalue references. It lets one object hand over its underlying resources to another instead of deep-copying them, dramatically reducing the cost of copying types that own heap allocations, file handles, or large buffers.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-move](https://en.cppreference.com/w/cpp/utility/move) / [cppreference-move-ctor](https://en.cppreference.com/w/cpp/language/move_constructor) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/05-move-semantics.md) | [Video Explanation]() | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/05-move-semantics-0.cpp) |  |


**Why was it introduced?**

- Before C++11, temporaries, function return values, and intermediate results could only be passed by copy-construct + destroy, paying a full deep copy even when the source was about to be discarded
- Types like `std::vector`, `std::string`, file-handle wrappers, and self-managed buffers have copy costs that scale with resource size, which becomes painful during container reallocation or function return
- A language-level mechanism was needed so that "an object that's about to be thrown away" could simply hand its resources to a new object instead of being copied and then destroyed

**How is move different from copy?**

- Copy: the new object **allocates its own fresh resource**, then **byte-by-byte copies** content from the source; the source remains intact
- Move: the new object **takes over** the source's internal pointer / handle directly, leaving the source in a "hollowed-out" valid-but-unspecified state where typically only destruction is safe
- Copy is O(resource size); move is usually O(1) — just a few pointer assignments

## I. Basic Usage and Scenarios

### What is std::move — It's a cast, not an actual "move"

The name `std::move` is highly misleading. It **does not move anything** and does not modify the object. All it does is **cast** an lvalue to an rvalue reference type so that overload resolution prefers the version taking `T&&`.

A close-to-real implementation looks like this.

```cpp
template <typename T>
typename std::remove_reference<T>::type&& move(T&& v) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(v);
}
```

The actual "resource transfer" is done by **the move constructor / move assignment operator**. `std::move` only **labels** the object as "I'm OK to be hollowed out"; the construction or assignment that follows is what really hollows it.

```cpp
Buffer a;
Buffer b = std::move(a); // std::move(a) just casts a to Buffer&&;
                         // the actual transfer happens in Buffer's move constructor
```

If a type **does not define** a move constructor / move assignment, `std::move` silently degrades to a copy and the compiler does not warn. This is the most common beginner trap.

### The Shape of Move Constructor / Move Assignment

Both have a fixed signature taking an rvalue reference `T&&` of the same type. The standard pattern is: **steal the source's resource pointer, then null out the source**, so destroying both objects later doesn't double-free.

```cpp
struct Buffer {
    int *data;

    Buffer() : data { new int[2] {0, 1} } { }

    // Move constructor: take over other's resource, then null out other
    Buffer(Buffer&& other) noexcept : data { other.data } {
        other.data = nullptr;
    }

    // Move assignment: release our old resource first, then take over,
    // then null out other
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }

    ~Buffer() {
        if (data) delete[] data;
    }
};
```

Three details to note.

- `noexcept` is practically required: `std::vector` and friends will **only use the move constructor during reallocation if it's declared noexcept**, otherwise they fall back to copy to preserve the strong exception guarantee
- Move assignment needs **self-assign check + release of old resource**, in that order
- The destructor must tolerate `data == nullptr`, because that's exactly the state of a moved-from object

### When the Compiler Auto-Generates Move (and When It Doesn't)

The compiler **auto-generates a default move constructor and move assignment** when

- The user has not declared any of: copy constructor, copy assignment, destructor, move constructor, move assignment

If the user **explicitly declares any one of these**, the move operations **will not** be auto-generated. (This is the motivation behind the "Rule of 5": once you customize one of them, you should review all five.)

```cpp
struct Foo {
    std::vector<int> v;
    // No special members declared -> move ctor/assign auto-generated,
    // forwards directly to v
};

struct Bar {
    std::vector<int> v;
    ~Bar() { /* any custom body */ }
    // Custom destructor -> move ctor/assign NOT auto-generated;
    // copies will fall back to deep copies of v
};
```

If the default member-wise move semantics is enough (e.g. all members are types like `std::vector` / `std::unique_ptr` that already support move), don't write your own. To force them in explicitly, use `= default`.

```cpp
struct Bar {
    std::vector<int> v;
    ~Bar() { }
    Bar(Bar&&) = default;
    Bar& operator=(Bar&&) = default;
};
```

### What Resource Ownership Transfer Actually Buys You

Back to the opening example: `Buffer` owns a heap-allocated chunk. Without move semantics, `process(Buffer())` triggers multiple "allocate + copy + destroy" rounds.

```cpp
Buffer process(Buffer buff) {  // construct on parameter
    return buff;               // construct on return value
}

Buffer b = process(Buffer());  // construct the temporary argument too
```

Once a move constructor exists, temporaries and local variables automatically pick the `Buffer&&` overload at consumption sites. The whole chain does exactly one `new int[2]`, all intermediate objects share the same buffer, and `delete[]` runs only once when the last object is destroyed. That's exactly what `05-move-semantics-0.cpp` wants you to **observe firsthand from the compiler output**.

The same idea generalizes to any "resource-owning" type: `std::unique_ptr`, file-handle wrappers, RAII network connections, large image / audio buffers — they all rely on move semantics to drive the transfer cost down to nearly zero.

## II. Important Notes

### A Moved-From Object Is in a "Valid-But-Unspecified" State

The standard only guarantees two things about a moved-from object.

- **The destructor can be called safely**
- The object satisfies its minimal type invariants (you, the implementer, decide what those are)

It does **not** guarantee any "original content" or any particular state. In `05-move-semantics-2.cpp` you'll see `b1.data_ptr() == nullptr` after the move — that's **because our implementation explicitly nulls it out**, not something the language enforces. Once a move has happened, do not **read** the moved-from object's contents; only **assign a new value** or **let it be destroyed**.

```cpp
Buffer b1;
Buffer b2 = std::move(b1);
// b1 is now in a valid-but-unspecified state:
b1.data_ptr();  // do not use b1's "business data" like this
b1 = Buffer();  // ok: reassign
// destruction at end of scope is also safe
```

### "Rule of 0 / 3 / 5"

- **Rule of 0**: prefer to delegate resource management to standard types that already support move (`std::vector`, `std::unique_ptr`, `std::string`...) and write zero special members yourself; the compiler handles everything for you
- **Rule of 3** (C++98): once you customize one of **destructor / copy constructor / copy assignment**, you usually need to customize the other two
- **Rule of 5** (C++11): on top of Rule of 3, also add **move constructor / move assignment** — once you take over resource management, all five special members should be designed together; otherwise you end up with inconsistencies like "copyable but not movable" or "movable but in a broken state after the move"

The safest strategy is still Rule of 0: let standard library types own the resources, and let your class just compose them.

### Don't Overuse std::move (Especially When Returning a Local)

When you write `return localObj;` directly, the compiler has **NRVO / RVO** (named / unnamed return value optimization) and can construct the object directly in the caller's frame — **not even a move is needed**. Writing `return std::move(localObj);` actually **suppresses NRVO**, forcing a move and giving you worse performance.

```cpp
Buffer good() {
    Buffer b;
    return b;            // ok: prefers NRVO, falls back to move
}

Buffer bad() {
    Buffer b;
    return std::move(b); // not recommended: kills NRVO, forces a move
}
```

Similarly, a **by-value parameter** is already a fresh object, so only consider `std::move` when forwarding it further out of the function. Applying `std::move` to a **const object** is also pointless: the result is a const rvalue reference, and overload resolution falls back to the copy constructor.

## III. Practice Code

### Practice Topics

- 0 - [Move constructor and trigger conditions - make buffer passing allocate only once](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/05-move-semantics-0.cpp)
- 1 - [Move assignment and trigger conditions - temporary / intermediate / explicit std::move](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/05-move-semantics-1.cpp)
- 2 - [Moving resources, not objects - compare object addresses vs. data pointers](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/05-move-semantics-2.cpp)

### Practice Code Auto-detection Command

```
d2x checker move-semantics
d2x checker move-semantics-2
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
