<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./11-inherited-constructors.html
[English]: ../en/cpp11/11-inherited-constructors.html

# 继承构造函数

继承构造函数是C++11 引入的一个语法特性 - 解决了在类继承结构中 派生类重复定义基类构造函数 的繁琐问题

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/using_declaration.html#Inheriting_constructors) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/11-inherited-constructors.md) | [Bili](https://www.bilibili.com/video/BV1bspBzFEEC) / [Youtube](https://youtu.be/p7vbY8XUKnY?si=GZUn9GSW68aU94A6) | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/11-inherited-constructors-0.cpp) |  |

**为什么引入?**

- 减少重复代码, 避免手动转发
- 提高代码的表达能力

## 一、基础用法和场景

### 复用基类的构造函数

在**继承构造函数**这个特性引入之前, 即使基类和派生的构造函数形式没有任何区别, 也需要重新定义, 这不仅造成了一定程度的代码重复, 而且也不够简洁。例如, 下面的`MyObject`就对每个`Base`中的构造函数做了重新实现

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

而用这个特性, 可以通过`using ObjectBase::ObjectBase;`直接继承基类中的构造函数, 避免这个手动转发的过程

```cpp
class MyObject : public ObjectBase {
public:
    using ObjectBase::ObjectBase;
    //...
};
```

这里需要注意的是, **构造函数继承** 的编译期隐式代码生成, 不仅仅是对构造函数的"单纯"复制, 而且在派生类中还有类似"自动重命名的效果 `ObjectBase` -> `MyObject` "。即:

```cpp
class MyObject : public ObjectBase {
public:
    // 可能的生成代码
    MyObject(int x) : ObjectBase(x) {}
    MyObject(double y) : ObjectBase(y) {}
};
```

### 类型的功能扩展

在很多特殊的场景下, 我们可能只想给某个类型追加额外的行为/方法, 而不改变其构造行为。这个时候就可使用继承构造

```cpp
class ObjectXXX : public Object {
public:
    using Object::Object;

    void your_method() { /* ... */ }
};
```

对一些类型做测试或调试时, 我们常常期望可以使用像`to_string()`之类的一些接口。如果在不方便直接修改源代码的情况下, 就可以使用 **继承构造函数** 的性质创建一个"具有一样接口"的新类型, 并追加一些方便调试的接口函数, 从而在有更方便的调试函数下实现间接测试。例如下面有个`Student`类:

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
通过实现`StudentDebug`并增加一些辅助函数, 这样更方便来获取调试信息

```cpp
class StudentDebug : public Student {
public:
    using Student::Student;

    std::string to_string() const {
        return "{ id: " + id + ", name: " + name
            + ", age: " + std::to_string(age) + " }";
    }

    void dump() const { /* 一些成绩细节 ... */ }
    void assert_valid() const {
        assert(score >= 0 && score <= 100);
        // ...
    }
};
```

同时, 在使用StudentDebug的时候, 不管是对象的创建还有原方法的使用都和Student保持了一致。所以对于这种 **只是增加行为, 而不改变原类型对象的构造形式的需求**, 使用继承构造能很大程度的简化代码

> 注: 一般这种方式可以保持同基类一样的 对象构造 + 行为/方法调用形式。但并不一定有一样的内存布局(例如新增虚方法), 并且类型判断上(RTTI)是不相等的

### 异常或错误类型标识和转发

在错误和异常处理时, 我们可以只定义一个基础的错误类型

```cpp
class ErrorBase {
public:
    ErrorBase() { }
    ErrorBase(const char *) { }
    ErrorBase(std::string) { }
    //...
};
```

在定义多个标识场景的错误类型时, 通过使用继承构造函数, 可以轻松的让他们保持和基础错误类型一样的构造形式。例如:

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

每个场景的错误, 对应一个错误类型, 不仅保持了错误对象构造的统一, 也非常适合配合C++的重载机制做错误类型的自动转发和处理。例如, 我们可以给每个错误类型实现对应的处理函数, 没有实现的类型将会使用基础类型对应的处理函数, 非常像很多编程语言中异常捕获和处理的设计。例如下面自定义的错误处理器:

```cpp

struct MyErrProcessor {
    static void process(ErrorBase err) { /* 基础处理 */ }
    static void process(ConfigError err) { /* 配置错误处理 */ }
    // ...
};

MyErrProcessor::process(errObj); // 自动匹配对应的错误处理函数

```

### 泛型装饰器和行为约束

**继承构造函数**不仅可以用于普通的继承中, 他还可以用于模板类型。例如, 下面定义的`NoCopy`中, 使用了`using T::T`对泛型T中的构造函数做继承。他的作用是在不改变目标对象的构造形式和使用接口下, 做一定的行为约束

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

在一些模块或场景中, 我们期望再对象创想创建后, 不能再复制的方式创建其他对象时, 就可以在定义时使用这个`NoCopy`装饰器/包装器, 通过包装器中的`delete`显示告诉编译器删除了**拷贝构造**和**拷贝赋值**, 也意味着对象不在拥有**拷贝语义**。例如:

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

这个时候`p1`和`p2`在接口的使用上都是一样的, 但是`p2`相对`p1`就少了可拷贝的属性

```cpp
p1.to_string(); // ok
p2.to_string(); // ok

auto p3 = p1; // ok (拷贝构造)
auto p4 = p2; // error (不能拷贝)
```

## 二、注意事项

### 优先考虑继承还是组合

由于本章是介绍**继承构造函数**的特性和使用方式, 它是和**继承**性质绑定的。所以, 从实现上是倾向用继承的方式来实现的。 但是从于目标功能上考虑, 往往使用继承和组合都是可以实现的, 他们更偏向是手段而不是目的, 所以选择需要结合具体的应用场景。

例如, 对于一些测试环境, 或**仅功能函数扩展, 无数据结构变动**的场景下, 使用继承配合**继承构造函数**是比较方便的, 还可以避免大量的函数转发。但是, 对于一些 要对少量特定接口做"拦截"或较复杂的场景, 现在(2025)主流是更倾向用组合代替继承的

- 复杂场景或要加一个中间层做特殊处理 -> 一般组合优于继承
- 简单功能扩展, 且需保留接口使用的一致 -> 一般继承优于组合

## 三、练习代码

### 练习代码主题

- 0 - [熟悉继承构造函数特性](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/11-inherited-constructors-0.cpp)
- 1 - [在功能扩展中的应用 - StudentTest](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/11-inherited-constructors-1.cpp)
- 2 - [在泛型模板中的应用 - NoCopy / NoMove 行为约束](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/11-inherited-constructors-2.cpp)

### 练习代码自动检测命令

```
d2x checker inherited-constructors
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)