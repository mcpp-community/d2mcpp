<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./04-rvalue-references.html
[English]: ../en/cpp11/04-rvalue-references.html

# 右值引用 - rvalue reference

右值引用 `T&&` 是 C++11 引入的一种新的引用类型, 用来精确**绑定到右值/将亡值**, 让编译器在重载决议时能区分 "可借走资源的临时对象" 和 "需要保留的具名对象", 是后续移动语义和完美转发的语法基石

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/reference) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/04-rvalue-references.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/04-rvalue-references.cpp) |  |


**为什么引入?**

- 在 C++11 之前, 想绑定一个临时对象只能用 `const T&`, 拿到的是只读视图, 无法在不复制的前提下复用它的资源
- 缺少一种能在重载层面 "认出右值" 的机制, 编译器无法把 "构造自临时对象" 和 "构造自普通对象" 这两条路径分开
- 为移动语义 (`std::move`) 和完美转发 (`std::forward`) 提供语法基础, 让 "把资源从将亡对象搬走" 成为可表达的语义

**左值和右值的区别?**

- **左值 (lvalue)**: 有名字、有持久存储位置、可以取地址的表达式, 例如已声明的变量
- **右值 (rvalue)**: 通常是字面量、临时对象、函数返回的非引用值, 生命周期短、不能直接取地址
- 经验判断: **能放在 `=` 左边、并能 `&` 取地址** 的一般是左值; 反之多为右值

## 一、基础用法和场景

### 左值 / 右值的判断

判断一个表达式是左值还是右值, 看它是不是有 "名字 + 持久身份"

```cpp
int a = 1;        // a 是左值
int b = a + 1;    // a + 1 是右值 (没有名字, 临时计算结果)

&a;        // ok: 左值可以取地址
// &(a + 1); // 错误: 右值不能取地址

int &lref  = a;       // ok: 左值引用绑定左值
// int &lref2 = a + 1; // 错误: 普通左值引用不能绑定右值
```

### 右值引用的声明和绑定

`T&&` 是右值引用的语法, 它**只能绑定到右值**

```cpp
int &&rref1 = 10;        // ok: 字面量是右值
int &&rref2 = a + 1;     // ok: 临时计算结果是右值

// int &&rref3 = a;      // 错误: 右值引用不能直接绑定左值
```

绑定后, 临时对象的生命周期会被**延长**到这个引用变量的作用域结束, 和 `const T&` 延长生命周期的规则一致, 但右值引用拿到的是**可写视图**

```cpp
struct Object {
    int data = 0;
};

const Object &cref = Object(); // 延长生命周期, 但只读
// cref.data = 1;              // 错误: 通过 const 引用不能修改

Object &&rref = Object();      // 延长生命周期, 且可写
rref.data = 1;                 // ok
```

这正是练习代码要验证的核心点: 让 `objRef.data = 1;` 能编译通过, 同时保证 `&objRef` 仍指向那个被延长生命周期的临时对象

### 函数重载中区分左右值

右值引用作为函数参数, 可以让编译器把 "传左值" 和 "传右值" 走两条不同的重载路径

```cpp
struct Object {
    Object() { std::cout << "Object()\n"; }
    Object(const Object&) { std::cout << "Object(const Object&)\n"; }
    Object(Object&&)      { std::cout << "Object(Object&&)\n"; }
};

void use(const Object&) { std::cout << "use lvalue\n"; }
void use(Object&&)      { std::cout << "use rvalue\n"; }

int main() {
    Object a;
    use(a);          // -> use lvalue   (a 是左值)
    use(Object());   // -> use rvalue   (临时对象是右值)
}
```

练习里的 `Object` 同时定义了拷贝构造 `Object(const Object&)` 和移动构造 `Object(Object&&)`, 就是为了通过打印来观察 "走的是哪条路径"

### 临时对象生命周期延长

下面是练习题的简化场景: 用引用绑定 `Object()` 这个纯右值, 临时对象的析构会被推迟到引用变量离开作用域

```cpp
{
    Object &&objRef = Object(); // 临时对象生命周期延长到这里
    objRef.data = 1;            // 通过右值引用修改它
} // 此处才析构
```

如果换成 `const Object &objRef = Object();`, 生命周期同样会延长, 但 `objRef.data = 1;` 这一行就编译失败了 — 这是 `const T&` 和 `T&&` 在这个场景下最直观的区别

## 二、注意事项

### 右值引用变量本身是左值

`int &&rref = 10;` 中, `rref` 这个**变量名**是有名字、可取地址的, 所以它在表达式里是**左值**, 不再是右值

```cpp
void use(const Object&) { std::cout << "lvalue path\n"; }
void use(Object&&)      { std::cout << "rvalue path\n"; }

Object &&rref = Object();
use(rref); // -> lvalue path  (rref 在表达式中是左值!)
```

如果想把它再当右值传出去, 需要用 `std::move(rref)` 显式转换, 这也是后续 "移动语义" 章节的入口

### const 引用和右值引用的重载选择

当同时存在 `const T&` 和 `T&&` 两个重载时, 编译器对**右值实参**会优先选择 `T&&` 版本

```cpp
void f(const Object&) { std::cout << "const &\n"; }
void f(Object&&)      { std::cout << "&&\n"; }

f(Object()); // -> &&     (右值优先匹配右值引用)

Object a;
f(a);        // -> const & (左值匹配 const 左值引用)
```

这条规则是 STL 容器 (如 `std::vector::push_back`) 能针对左右值分别走 "拷贝" 和 "移动" 路径的基础

### 不要对右值引用变量名再施加 `&&`

`T&&` 在**模板参数推导**场景下会变成 "万能引用 / 转发引用", 行为和这里的纯右值引用不同 — 同样的语法在 `template<typename T> void f(T&&)` 里既能绑左值也能绑右值. 这部分属于完美转发的内容, 在后续章节展开, 这里只需记住: **非模板上下文中**的 `T&&` 是 "只接受右值" 的右值引用

```cpp
void g(Object&& o);             // 只接受右值
template<typename T> void h(T&&); // 万能引用, 左右值都接受
```

### 右值引用是移动语义的入口, 不是终点

本章重点是 **值类别 + 引用绑定** 这一层机制. 真正利用右值引用 "把资源搬走" 的部分 (移动构造 / 移动赋值 / `std::move`) 在 ch05 展开. 不过练习代码里 `Object(Object&&)` 这个移动构造的打印, 已经能让你直观看到 "右值实参 -> 走移动路径" 的全过程

## 三、练习代码

### 练习代码主题

- 0 - [使用右值引用延长临时对象生命周期并修改其值](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/04-rvalue-references.cpp)

### 练习代码自动检测命令

```
d2x checker rvalue-references
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
