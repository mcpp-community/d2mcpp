<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ../../cpp11/11-inherited-constructors.html
[English]: ./11-inherited-constructors.html

# Inherited Constructors

Inherited constructors are a syntactic feature introduced in C++11 that solves the tedious problem of derived classes repeatedly defining base class constructors in class inheritance structures.

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/using_declaration.html#Inheriting_constructors) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/en/src/cpp11/11-inherited-constructors.md) | [Bili](https://www.bilibili.com/video/BV1bspBzFEEC) / [Youtube](https://youtu.be/p7vbY8XUKnY?si=GZUn9GSW68aU94A6) | [Practice Code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/11-inherited-constructors-0.cpp) |  |

**Why was it introduced?**

- Reduce repetitive code, avoid manual forwarding
- Improve code expressiveness

## I. Basic Usage and Scenarios

### Reusing Base Class Constructors

Before the **inherited constructors** feature was introduced, even when base and derived class constructors had identical forms, they still needed to be redefined. This not only caused code duplication but also lacked conciseness. For example, in the following code, `MyObject` reimplements each constructor from `Base`:

```cpp
class ObjectBase {
    //...
public:
    ObjectBase(int) {}
    ObjectBase(double) {}
};

class MyObject : public ObjectBase {
public:
    MyObject(int x) : ObjectBase(x) {}
    MyObject(double y) : ObjectBase(y) {}
    //...
};
```

With this feature, you can directly inherit constructors from the base class using `using ObjectBase::ObjectBase;`, avoiding this manual forwarding process:

```cpp
class MyObject : public ObjectBase {
public:
    using ObjectBase::ObjectBase;
    //...
};
```

It's important to note that **constructor inheritance** during compile-time implicit code generation is not just a "simple" copy of constructors, but also has an effect similar to "automatic renaming" in the derived class (`ObjectBase` -> `MyObject`). That is:

```cpp
class MyObject : public ObjectBase {
public:
    // Possible generated code
    MyObject(int x) : ObjectBase(x) {}
    MyObject(double y) : ObjectBase(y) {}
};
```

### Type Functionality Extension

In many special scenarios, we might want to add additional behavior/methods to a type without changing its construction behavior. This is where inherited constructors can be used:

```cpp
class ObjectXXX : public Object {
public:
    using Object::Object;

    void your_method() { /* ... */ }
};
```

When testing or debugging certain types, we often wish to use interfaces like `to_string()`. If modifying the source code directly is inconvenient, we can use the **inherited constructors** feature to create a new type with "the same interface" and add some convenient debugging interface functions, thus achieving indirect testing with more convenient debugging functions. For example, consider this `Student` class:

```cpp
class Student {
protected:
    //...
    double score;
public:
    string id;
    string name;
    uint age;

    Student(string id, string name);
    Student(string id, string name, uint age);
    Student(string id, ...);
};
```

By implementing `StudentDebug` and adding some helper functions, it becomes easier to obtain debugging information:

```cpp
class StudentDebug : public Student {
public:
    using Student::Student;

    std::string to_string() const {
        return "{ id: " + id + ", name: " + name
            + ", age: " + std::to_string(age) + " }";
    }

    void dump() const { /* some score details ... */ }
    void assert_valid() const {
        assert(score >= 0 && score <= 100);
        // ...
    }
};
```

When using `StudentDebug`, both object creation and original method usage remain consistent with `Student`. Therefore, for requirements that **only add behavior without changing the original type's object construction form**, using inherited constructors can greatly simplify code.

> Note: Generally, this approach can maintain the same object construction + behavior/method invocation form as the base class. However, it doesn't necessarily have the same memory layout (e.g., adding virtual methods), and type judgment (RTTI) is not equal.

### Exception or Error Type Identification and Forwarding

In error and exception handling, we can define only a basic error type:

```cpp
class ErrorBase {
public:
    ErrorBase() { }
    ErrorBase(const char *) { }
    ErrorBase(std::string) { }
    //...
};
```

When defining error types for multiple identification scenarios, using inherited constructors easily allows them to maintain the same construction form as the base error type. For example:

```cpp
class ConfigError : public ErrorBase {
public:
    using ErrorBase::ErrorBase;
};

class RuntimeError : public ErrorBase {
public:
    using ErrorBase::ErrorBase;
};

class IoError : public ErrorBase {
public:
    using ErrorBase::ErrorBase;
};
```

Each scenario's error corresponds to an error type, not only maintaining unified error object construction but also being well-suited for automatic error type forwarding and processing with C++'s overloading mechanism. For example, we can implement corresponding processing functions for each error type, and types without implementations will use the base type's processing function, similar to exception catching and handling designs in many programming languages. For example, a custom error processor:

```cpp
struct MyErrProcessor {
    static void process(ErrorBase err) { /* base processing */ }
    static void process(ConfigError err) { /* config error processing */ }
    // ...
};

MyErrProcessor::process(errObj); // Automatically matches corresponding error processing function
```

### Generic Decorators and Behavior Constraints

**Inherited constructors** can be used not only in ordinary inheritance but also in template types. For example, in the following `NoCopy` definition, `using T::T` is used to inherit constructors from generic type T. Its purpose is to apply certain behavior constraints without changing the target object's construction form and usage interface:

```cpp
template <typename T>
class NoCopy : public T {
public:
    using T::T;

    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;
    // ...
};
```

In some modules or scenarios, when we want objects to not be created by copying after initial creation, we can use this `NoCopy` decorator/wrapper during definition. The wrapper's `delete` explicitly tells the compiler to delete **copy construction** and **copy assignment**, meaning the object no longer has **copy semantics**. For example:

```cpp
class Point {
    double mX, mY;
public:
    Point() : mX { 0 }, mY { 0 } { }
    Point(double x, double y) : mX { x }, mY { y } { }

    string to_string() const {
        return "{ " + std::to_string(mX)
            + ", " + std::to_string(mY) + " }";
    }
};

Point p1(1, 2);
NoCopy<Point> p2(2, 3);
```

In this case, both `p1` and `p2` have the same interface usage, but `p2` lacks the copyable property compared to `p1`:

```cpp
p1.to_string(); // ok
p2.to_string(); // ok

auto p3 = p1; // ok (copy construction)
auto p4 = p2; // error (cannot copy)
```

## II. Important Notes

### Prefer Inheritance or Composition

Since this chapter introduces the **inherited constructors** feature and usage, it's bound to the **inheritance** nature. Therefore, implementation-wise, it tends to use inheritance. However, considering the target functionality, both inheritance and composition can often achieve the goal; they are more like means rather than ends, so the choice should be based on specific application scenarios.

For example, for testing environments or scenarios involving **only functional extension without data structure changes**, using inheritance with **inherited constructors** is more convenient and can avoid extensive function forwarding. However, for scenarios requiring "interception" of specific interfaces or more complex situations, the mainstream approach (as of 2025) tends to prefer composition over inheritance.

- Complex scenarios or requiring an intermediate layer for special processing -> generally composition is better than inheritance
- Simple functional extension requiring consistent interface usage -> generally inheritance is better than composition

## III. Practice Code

### Practice Code Topics

- 0 - [Familiarize with Inherited Constructors Feature](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/11-inherited-constructors-0.cpp)
- 1 - [Application in Functional Extension - StudentTest](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/11-inherited-constructors-1.cpp)
- 2 - [Application in Generic Templates - NoCopy / NoMove Behavior Constraints](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/11-inherited-constructors-2.cpp)

### Practice Code Auto-detection Command

```
d2x checker inherited-constructors
```

## IV. Additional Resources

- [Discussion Forum](https://forum.d2learn.org/category/20)
- [d2mcpp Tutorial Repository](https://github.com/mcpp-community/d2mcpp)
- [Tutorial Video List](https://space.bilibili.com/65858958/lists/5208246)
- [Tutorial Support Tool - xlings](https://github.com/d2learn/xlings)
