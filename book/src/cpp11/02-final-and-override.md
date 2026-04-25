<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./02-final-and-override.html
[English]: ../en/cpp11/02-final-and-override.html

# final 和 override - 虚函数重写控制

final 和 override 是 C++11 引入的两个 **上下文相关标识符**, 用于在虚函数继承中显式表达 **重写** 和 **封口** 的意图, 让编译器在编译期就暴露原本只能在运行期才能发现的多态错位问题

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-final](https://en.cppreference.com/w/cpp/language/final) / [cppreference-override](https://en.cppreference.com/w/cpp/language/override) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/02-final-and-override.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/02-final-and-override-0.cpp) |  |


**为什么引入?**

- 在 C++11 之前, "派生类有没有真的重写到基类的虚函数" 完全靠程序员自己核对签名, 一个参数写错就会从重写变成隐藏, 编译器不会报错
- 缺少标准方式来表达 "这个类型 / 这条多态链到此为止" 的设计意图
- 让虚函数的设计契约变得可读、可校验

**两者的语义区别?**

- override: 加在派生类成员函数后, 显式声明 "这个函数是在重写基类的虚函数", 让编译器协助检查
- final: 加在虚函数后表示 "这个虚函数不能再被派生类重写", 加在类后表示 "这个类不能再被继承"

## 一、基础用法和场景

### override - 显式声明重写

不加 override 时, 派生类哪怕签名写错也只是 "新增了一个普通函数", 多态行为悄悄丢失

```cpp
struct Base {
    virtual void func(int) { }
};

struct Derived : Base {
    void func(double) { } // 本来想重写, 但参数类型不一致, 实际是新声明了一个函数
};
```

加上 override 后, 同样的错误会在编译期被拒绝

```cpp
struct Derived : Base {
    void func(double) override; // 错误: 没有签名匹配的基类虚函数
};
```

只有签名 (返回类型 + 参数列表 + cv 限定 + 引用限定) 完全匹配的基类虚函数, 才会让 override 通过

```cpp
struct Base {
    virtual void func(int);
};

struct Derived : Base {
    void func(int) override; // ok
};
```

### final - 禁止后续重写或派生

final 有两种用法, 作用对象不同

**修饰虚函数 - 截断多态链**

```cpp
struct A {
    virtual void func() final { }
};

struct B : A {
    void func() override; // 错误: A::func 已被 final, 不能再重写
};
```

**修饰类 - 禁止派生**

```cpp
struct B final { };

struct C : B { }; // 错误: B 已被 final, 不能被继承
```

### final + 纯虚 - 不可改写的模板方法 (NVI)

把外层接口用 `virtual ... final` 锁住, 把可定制的步骤暴露成纯虚函数, 就得到一个 "执行顺序不可改、但每一步可定制" 的稳定接口. 这是 **非虚接口惯用法 (Non-Virtual Interface)** 的一种简洁写法

```cpp
struct AudioPlayer {
    virtual void play() final {  // 子类不能改写 play 的整体流程
        init_audio_params();
        play_audio();
    }
private:
    virtual void init_audio_params() = 0; // 留给子类定制
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

调用方拿到的是统一的 `AudioPlayer::play()`, 不同格式的播放器只需实现两个钩子. 这种结构在做插件式 / 协议式接口时很常见

### 上下文相关标识符

override 和 final 既不是保留字, 也不是关键字, 而是 **上下文相关标识符 (context-sensitive identifiers)**. 只有出现在虚函数声明或类声明的特定位置时才被解释为这两种语义, 普通位置可以正常用作变量名、类型名、命名空间名等

```cpp
B override; // ok: 这里 override 只是个普通变量名
B final;    // ok: 这里 final 只是个普通变量名
```

这也是 C++ 标准为了向后兼容做的折中: 已有代码里用 override / final 当标识符的, 不会因为升级到 C++11 而编译失败

## 二、注意事项

### override 必须有签名一致的基类虚函数

派生类成员函数加上 override 后, 编译器会要求在某个基类中存在 **签名一致的虚函数**, 否则编译报错. 这是 override 最核心的价值: 把 "重写错位" 这种 silent bug 从运行期提前到编译期

```cpp
struct A {
    virtual void func1() { }
    void func2() { } // 注意: 不是 virtual
};

struct B : A {
    void func1() override; // ok
    void func2() override; // 错误: A::func2 不是 virtual
};
```

### final 类是 "封口" - 慎用

final 类不能被继承, 哪怕只是想派生一个加几个辅助方法的子类也不行. 给一个类加 final, 实际上是在做 "这个类型在设计上就是叶子节点" 的承诺, 需要谨慎. 经验法则:

- 业务上明确不希望被继续派生 (例如错误类型 / 框架内部实现类 / 单例) -> 适合 final
- 通用基类 / 框架预留的可扩展点 -> 不要轻易加 final

### final 只能用在虚函数上

普通成员函数本来就不能被 override, 给它加 final 没有意义, 编译器也会拒绝

```cpp
struct A {
    void func() final; // 错误: 非虚函数上不能用 final
};
```

### override 和 final 可以同时出现

如果某个虚函数既要重写基类版本, 又要禁止再被派生类继续重写, 可以两者一起加

```cpp
struct B : A {
    void func() override final; // 重写 A::func, 同时禁止 C 再重写
};
```

## 三、练习代码

### 练习代码主题

- 0 - [熟悉 override 的使用](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/02-final-and-override-0.cpp)
- 1 - [熟悉 final 的使用](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/02-final-and-override-1.cpp)
- 2 - [final + 模板方法 - AudioPlayer / WAV / MP3 / OGG](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/02-final-and-override-2.cpp)

### 练习代码自动检测命令

```
d2x checker final-and-override
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
