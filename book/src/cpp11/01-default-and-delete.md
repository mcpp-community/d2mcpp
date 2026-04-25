<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./01-default-and-delete.html
[English]: ../en/cpp11/01-default-and-delete.html

# default 和 delete - 显式控制特殊成员函数

`= default` 和 `= delete` 是 C++11 引入的两种 **函数定义方式**, 让程序员可以在源码层面显式声明 "这个特殊成员我要编译器生成默认实现" 或 "这个函数禁止被调用", 把原本只能靠编译器隐式规则推断的特殊成员控制权重新交还给设计者

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/function#Deleted_functions) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/01-default-and-delete.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-0.cpp) |  |


**为什么引入?**

- 在 C++11 之前, 只要类里写了任何一个用户定义的构造函数, 编译器就会停止生成默认构造函数, 没有显式办法把它"找回来"
- 没有标准方式表达 "这个特殊成员我故意不要 - 谁调用就编译错". 老办法是 "把拷贝构造声明成 private 且不实现", 错误信息晦涩、还要等到链接期才能暴露
- 不同编译器对 "什么时候自动生成 / 自动删除特殊成员" 的隐式规则容易让人记错, 显式标记可以让意图直接写在代码里

**= default 和 = delete 的语义?**

- `= default`: 让编译器按规则生成这个特殊成员的默认实现 (默认构造 / 析构 / 拷贝 / 移动 / [C++20] 比较运算符 等), 等价于 "我要这个生成版本, 别因为我写了别的成员就把它隐式删掉"
- `= delete`: 显式禁止某个函数 - 任何对它的调用 / 重载解析选中它, 都会在 **编译期** 直接报错, 错误信息明确指向 "use of deleted function"

## 一、基础用法和场景

### 显式 default - 把被屏蔽的默认构造要回来

只要类里出现任何用户定义的构造函数, 编译器就不再为它合成默认构造. 下面 `B` 的写法就直接禁掉了 `B b;` 这种调用

```cpp
struct B {
    B(int x) { std::cout << "B(int x)" << std::endl; }
};

B b;        // 错误: 没有默认构造函数
B b2(10);   // ok
```

用 `= default` 显式声明一份默认构造, 就能把它要回来, 又不影响已经写好的有参构造

```cpp
struct B {
    B() = default;                                       // 显式要求生成默认构造
    B(int x) { std::cout << "B(int x)" << std::endl; }   // 用户定义的有参构造
};

B b;        // ok
B b2(10);   // ok
```

类似的, `C` 里如果同时有无参构造和带默认值的有参构造, 这两者会在 `C c;` 的重载解析中产生 **二义性**. 把无参版本写成 `= default` 并去掉有参的默认值, 意图就清晰了

```cpp
struct C {
    C() = default;
    C(int x) { std::cout << "C(int x): " << x << std::endl; }
};

C c1;      // 调用 C()
C c2(1);   // 调用 C(int)
```

### 显式 delete - 实现不可拷贝对象

`std::unique_ptr` 最关键的语义就是 "独占所有权 -> 不可拷贝, 但可移动". 自己手写一个简化版, 只要把拷贝相关的两个特殊成员 `= delete` 掉、把移动相关的两个写成 `= default` 即可

```cpp
struct UniquePtr {
    void *dataPtr;
    UniquePtr() = default;

    UniquePtr(const UniquePtr&) = delete;             // 禁止拷贝构造
    UniquePtr& operator=(const UniquePtr&) = delete;  // 禁止拷贝赋值

    UniquePtr(UniquePtr&&) = default;                 // 允许移动构造
    UniquePtr& operator=(UniquePtr&&) = default;      // 允许移动赋值
};

UniquePtr a;
UniquePtr b = a;             // 错误: copy ctor 已被 delete
UniquePtr c = std::move(a);  // ok: move ctor
```

可以用类型萃取在编译期直接验证语义是否符合预期

```cpp
static_assert(std::is_copy_constructible<UniquePtr>::value == false, "");
static_assert(std::is_copy_assignable<UniquePtr>::value    == false, "");
static_assert(std::is_move_constructible<UniquePtr>::value == true,  "");
static_assert(std::is_move_assignable<UniquePtr>::value    == true,  "");
```

### 用 = delete 在重载集中"屏蔽"特定参数类型

`= delete` 不止能用在特殊成员上, 任何普通函数的某个重载都可以删掉. 一个常见模式是 **阻止隐式转换**, 让调用者用 "错误" 的参数类型时直接编译失败

```cpp
void func(int x) {
    std::cout << "x = " << x << std::endl;
}

// 显式删除 float 参数的重载, 否则 func(1.1f) 会被隐式转换成 int
void func(float) = delete;

func(1);       // ok: 走 int 重载
func(1.1f);    // 错误: 调用了 deleted function
```

如果不写这个 deleted 重载, `func(1.1f)` 会悄悄发生 `float -> int` 的窄化转换, 截断掉 `0.1`. 用 `= delete` 把它从重载集中移除后, 错误信息就明确多了: "use of deleted function 'void func(float)'"

### default / delete 适用的成员清单

`= default` 可以用于编译器原本就能合成的 "特殊成员函数":

- 默认构造 (无参)
- 析构
- 拷贝构造 / 拷贝赋值
- 移动构造 / 移动赋值
- (C++20) `<=>` 等比较运算符

`= delete` 则没有这种限制 - 任何函数声明 (普通函数、成员函数、模板特化、特殊成员) 都可以写 `= delete`

## 二、注意事项

### = default 不一定意味着 "trivial"

写了 `= default` 只是表示 "由编译器生成", 但生成出来的版本是否是 trivial / 是否是 noexcept, 取决于这个类的基类和成员. 例如基类的拷贝构造非 trivial, 那么派生类即使写 `= default`, 它的拷贝构造也是 non-trivial

```cpp
struct HasString {
    std::string s;        // string 的 copy ctor 不是 trivial
    HasString(const HasString&) = default;
};

static_assert(!std::is_trivially_copy_constructible<HasString>::value, "");
```

如果你的代码依赖 "trivial" 这个属性 (例如 memcpy 拷贝、放入 union), 不要只看到 `= default` 就直接下结论, 用 `std::is_trivially_*` 类型萃取去验证

### delete 也可以用在普通函数上

`= delete` 不是特殊成员的专利. 任何普通函数都可以删掉, 既能用来禁止某个重载, 也能用在函数模板里禁止某些特化

```cpp
template <typename T>
void only_int(T) = delete;   // 默认全部禁掉

template <>
void only_int<int>(int x) {  // 只允许 int
    std::cout << x << std::endl;
}

only_int(1);       // ok
only_int(1.0);     // 错误: 调用 deleted function
```

### 不要把 deleted 函数放成 private

老式的 "禁拷贝" 写法是把 copy ctor / copy assign 声明成 private 且不实现. 这个写法在 C++11 之后已经过时, 应该统一改成 `= delete` (放在 public 区), 原因:

- `= delete` 在重载解析阶段就报错, 错误信息更明确; private + 未实现要等到链接期才暴露
- 放在 public 让所有访问者拿到的错误信息一致, 不会因为 friend / 同类成员里调用而出现不同的错误形态

### Rule of 0 / 3 / 5 - 设计类时的指导

`= default` / `= delete` 真正的设计价值, 是配合 **Rule of 0 / 3 / 5**:

- **Rule of 0**: 类不直接管理资源, 全靠成员的 RAII (例如 `std::string`, `std::vector`, `std::unique_ptr`) -> 不写任何特殊成员, 让编译器自动合成
- **Rule of 3 (C++98)**: 如果实现了拷贝构造、拷贝赋值、析构中的任何一个, 通常另外两个也要实现
- **Rule of 5 (C++11)**: 引入移动语义后, 把移动构造和移动赋值也加进来 - 一旦你显式定义/删除/默认了其中一个, 最好把全部 5 个都写出来, 避免被编译器的隐式规则坑到

## 三、练习代码

### 练习代码主题

- 0 - [显式指定构造函数生成行为](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-0.cpp)
- 1 - [实现不可拷贝但可移动的 UniquePtr](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-1.cpp)
- 2 - [用 = delete 屏蔽特定参数类型的重载](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-2.cpp)

### 练习代码自动检测命令

```
d2x checker default-and-delete
d2x checker default-and-delete-1
d2x checker default-and-delete-2
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
