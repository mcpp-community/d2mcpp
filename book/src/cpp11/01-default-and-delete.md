<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./01-default-and-delete.html
[English]: ../en/cpp11/01-default-and-delete.html

# default 和 delete - 显式控制特殊成员函数

`= default` 和 `= delete` 是 C++11 引入的两种 **函数定义语法**, 用于在源码层面显式指定特殊成员函数采用编译器生成的默认实现, 或显式禁止某一函数参与调用与重载决议. 二者将特殊成员函数的生成控制权从编译器的隐式规则交还给类的设计者

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/function#Deleted_functions) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/01-default-and-delete.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-0.cpp) |  |


**为什么引入?**

- 在 C++11 之前, 只要类中定义了任意一个用户声明的构造函数, 编译器即停止合成默认构造函数, 且缺乏单独控制某一构造函数是否生成的手段
- 缺乏表达 "禁用某一特殊成员" 的标准语法. 传统做法是将拷贝构造声明为 private 且不提供定义, 其错误信息晦涩, 且需延迟至链接期才能暴露
- 各编译器关于 "特殊成员何时自动生成或自动删除" 的隐式规则存在差异, 易产生记忆偏差; 显式标记可将设计意图直接表达于代码中

**= default 和 = delete 的语义?**

- `= default`: 指示编译器按语言规则合成该特殊成员的默认实现 (默认构造 / 析构 / 拷贝 / 移动 / [C++20] 比较运算符 等), 用于在已声明其他成员的情况下显式保留该成员, 避免其被隐式删除
- `= delete`: 显式禁用某一函数. 任何对其的调用, 或重载决议选中该函数的情形, 均在 **编译期** 报错, 错误信息明确指向 "use of deleted function"

## 一、基础用法和场景

### 显式 default - 恢复被抑制的默认构造函数

只要类中出现任意用户声明的构造函数, 编译器即不再为其合成默认构造函数. 下例中 `B` 的定义因此禁用了 `B b;` 形式的调用

```cpp
struct B {
    B(int x) { std::cout << "B(int x)" << std::endl; }
};

B b;        // 错误: 没有默认构造函数
B b2(10);   // ok
```

通过 `= default` 显式声明一份默认构造函数即可将其恢复, 同时不影响已声明的有参构造函数

```cpp
struct B {
    B() = default;                                       // 显式要求生成默认构造
    B(int x) { std::cout << "B(int x)" << std::endl; }   // 用户定义的有参构造
};

B b;        // ok
B b2(10);   // ok
```

类似地, 当 `C` 同时具有无参构造函数与带默认实参的有参构造函数时, 二者在 `C c;` 的重载决议中产生 **二义性**. 将无参版本声明为 `= default` 并移除有参版本的默认实参, 即可消除二义性并明确意图 (或只保留默认参数版本也可)

```cpp
struct C {
    //C() = default;
    C(int x = 1) { std::cout << "C(int x): " << x << std::endl; }
};

C c1;      // 调用 C()
C c2(1);   // 调用 C(int)
```

### 显式 delete - 实现不可拷贝对象

`std::unique_ptr` 的核心语义为 "独占所有权 -> 不可拷贝, 但可移动". 实现一个简化版本时, 只需将两个拷贝相关的特殊成员声明为 `= delete`, 并将两个移动相关的特殊成员声明为 `= default`

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

可借助类型萃取在编译期验证上述语义是否符合预期

```cpp
static_assert(std::is_copy_constructible<UniquePtr>::value == false, "");
static_assert(std::is_copy_assignable<UniquePtr>::value    == false, "");
static_assert(std::is_move_constructible<UniquePtr>::value == true,  "");
static_assert(std::is_move_assignable<UniquePtr>::value    == true,  "");
```

### 用 = delete 在重载集中"屏蔽"特定参数类型

`= delete` 不限于特殊成员, 普通函数的任一重载亦可被删除. 一种常见用法是 **阻止隐式转换**, 使调用方传入非预期的参数类型时直接编译失败

```cpp
void func(int x) {
    std::cout << "x = " << x << std::endl;
}

// 显式删除 float 参数的重载, 否则 func(1.1f) 会被隐式转换成 int
void func(float) = delete;

func(1);       // ok: 走 int 重载
func(1.1f);    // 错误: 调用了 deleted function
```

若不声明该 deleted 重载, `func(1.1f)` 将隐式发生 `float -> int` 的窄化转换并截断 `0.1`. 通过 `= delete` 将其从重载集中移除后, 错误信息更为明确: "use of deleted function 'void func(float)'"

### default / delete 适用的成员清单

`= default` 仅适用于编译器本可合成的特殊成员函数:

- 默认构造 (无参)
- 析构
- 拷贝构造 / 拷贝赋值
- 移动构造 / 移动赋值
- (C++20) `<=>` 等比较运算符

`= delete` 不受此限制 - 任意函数声明 (普通函数、成员函数、模板特化、特殊成员) 均可声明为 `= delete`

## 二、真实案例 - STL 中的 default/delete

> 前述示例用于说明语法, 而 `= default` / `= delete` 的实际价值在标准库自身的实现中体现得最为直接。下面从仓库内置的 [MSVC STL](https://github.com/mcpp-community/d2mcpp/tree/main/msvc-stl) 中选取两段代码, 分别演示「以 `= delete` 表达不可拷贝」与「`= default` / `= delete` 并用」; `_CONSTEXPR23`、`_Mypair` 等为库内部的宏与命名, 阅读时关注 `= default` / `= delete` 即可

### delete 实现不可拷贝:std::unique_ptr

`std::unique_ptr` 的核心语义为 "独占所有权 -> 不可拷贝、只可移动", 与 `## 一`「显式 delete - 实现不可拷贝对象」中手写的简化 `UniquePtr` 一致 —— 标准库的实际实现采用相同方式: 将两个拷贝成员声明为 `= delete`, 移动成员则保留可用

```cpp
// MSVC STL · msvc-stl/stl/inc/memory (有删节) —— class unique_ptr
    // 移动构造 / 移动赋值:保留, 转移所有权(此处为库的模板化定义)
    _CONSTEXPR23 unique_ptr(unique_ptr&& _Right) noexcept;
    _CONSTEXPR23 unique_ptr& operator=(unique_ptr&& _Right) noexcept;

    // 拷贝构造 / 拷贝赋值:= delete, 任何拷贝尝试都在编译期报错
    unique_ptr(const unique_ptr&)            = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
```

将拷贝两件套声明为 `= delete`、保留移动两件套可用, 即精确表达出一个 "独占、可移动、不可复制" 的类型。前文手写的 `UniquePtr` 与之完全对应

### default / delete 并用:shared_ptr 的基类 _Ptr_base

`shared_ptr` 和 `weak_ptr` 共用的基类 `_Ptr_base` 同时使用了两种标记:基类自身禁止直接拷贝 (`= delete`), 而默认构造和析构则交由编译器合成 (`= default`)。这是 `## 一`「适用清单」中相应规则的实际体现

```cpp
// MSVC STL · msvc-stl/stl/inc/memory (有删节) —— class _Ptr_base
    _Ptr_base(const _Ptr_base&)            = delete; // 基类禁止直接拷贝
    _Ptr_base& operator=(const _Ptr_base&) = delete;

protected:
    constexpr _Ptr_base() noexcept = default;        // 默认构造交给编译器
    ~_Ptr_base()                   = default;        // 析构交给编译器
```

`= delete` 用于禁止不应发生的操作, `= default` 用于显式保留应自动生成的成员 —— 同一类中两种标记各司其职, 将 "允许哪些操作、禁止哪些操作" 完整表达于代码中

> 小结: 不可拷贝、独占所有权、显式声明默认成员等设计, 在标准库内部均依赖本章介绍的 default / delete 工具实现。这也是 C++11 引入二者的核心动机之一

## 三、注意事项

### = default 不一定意味着 "trivial"

`= default` 仅表示 "由编译器生成", 而生成版本是否为 trivial、是否为 noexcept, 取决于该类的基类与成员. 例如, 当基类的拷贝构造非 trivial 时, 派生类即使声明 `= default`, 其拷贝构造仍为 non-trivial

```cpp
struct HasString {
    std::string s;        // string 的 copy ctor 不是 trivial
    HasString(const HasString&) = default;
};

static_assert(!std::is_trivially_copy_constructible<HasString>::value, "");
```

若代码依赖 "trivial" 属性 (例如以 memcpy 拷贝、置入 union), 不应仅凭 `= default` 即作判断, 而应通过 `std::is_trivially_*` 类型萃取加以验证

### delete 同样适用于普通函数

`= delete` 并不限于特殊成员. 任意普通函数均可被删除, 既可用于禁止某一重载, 也可用于在函数模板中禁止特定特化

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

### deleted 函数不应声明为 private

传统的 "禁止拷贝" 写法是将 copy ctor / copy assign 声明为 private 且不提供定义. 该写法在 C++11 之后已不推荐, 应统一改用 `= delete` (置于 public 区), 原因如下:

- `= delete` 在重载决议阶段即报错, 错误信息更为明确; private + 未定义的方式需延迟至链接期才暴露
- 置于 public 可使所有访问者获得一致的错误信息, 不会因 friend / 同类成员中的调用而产生不同的错误形态

### Rule of 0 / 3 / 5 - 设计类时的指导

`= default` / `= delete` 的核心设计价值在于配合 **Rule of 0 / 3 / 5**:

- **Rule of 0**: 类不直接管理资源, 完全依赖成员的 RAII (例如 `std::string`, `std::vector`, `std::unique_ptr`) -> 不声明任何特殊成员, 由编译器自动合成
- **Rule of 3 (C++98)**: 若实现了拷贝构造、拷贝赋值、析构之一, 通常需一并实现另外两个
- **Rule of 5 (C++11)**: 引入移动语义后, 移动构造与移动赋值亦纳入其中 - 一旦显式定义、删除或默认其中之一, 应将全部 5 个成员一并声明, 以避免编译器隐式规则带来的非预期行为

## 四、练习代码

### 练习代码主题

- 0 - [显式指定构造函数生成行为](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-0.cpp)
- 1 - [实现不可拷贝但可移动的 UniquePtr](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-1.cpp)
- 2 - [用 = delete 屏蔽特定参数类型的重载](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/01-default-and-delete-2.cpp)

### 练习代码自动检测命令

<details>
<summary>还没有 d2x?点击展开获取方式</summary>

```bash
# 1. 安装 xlings(Linux / macOS)
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.sh | bash
# Windows PowerShell:
# irm https://raw.githubusercontent.com/openxlings/xlings/main/tools/other/quick_install.ps1 | iex

# 2. 安装 d2x 并拉取本教程
xlings install d2x -y
d2x install d2mcpp

# 3. 进入项目目录 & 运行检查命令
cd d2mcpp
```

</details>

```
d2x checker default-and-delete
d2x checker default-and-delete-1
d2x checker default-and-delete-2
```

## 五、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [mcpp-community 社区](https://github.com/mcpp-community)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)
- [教程检测工具-d2x](https://github.com/d2learn/d2x)