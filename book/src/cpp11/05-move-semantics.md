<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./05-move-semantics.html
[English]: ../en/cpp11/05-move-semantics.html

# 移动语义 - move semantics

移动语义是 C++11 在右值引用基础上引入的一种**资源所有权转移**机制, 让对象之间在传递时可以"搬资源"而不是"复制资源", 显著降低了带堆分配 / 文件句柄 / 大块缓冲区类型的拷贝开销

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference-move](https://en.cppreference.com/w/cpp/utility/move) / [cppreference-move-ctor](https://en.cppreference.com/w/cpp/language/move_constructor) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/05-move-semantics.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/05-move-semantics-0.cpp) |  |


**为什么引入?**

- 在 C++11 之前, 临时对象 / 函数返回值 / 中间结果在传递时只能走拷贝构造 + 析构, 即使原对象立刻就要销毁, 也要付出一次完整的深拷贝
- 像 `std::vector`, `std::string`, 文件句柄, 自管理 buffer 这类拥有堆资源的类型, 拷贝代价和资源大小成正比, 在容器扩容 / 函数返回时尤其明显
- 需要一种语言层面的方式, 让"反正马上要扔掉的对象"把自己持有的资源直接交给新对象, 而不是复制一份再销毁

**移动和拷贝的区别?**

- 拷贝: 新对象**自己分配一块新资源**, 然后把源对象的资源**逐字节复制**进来, 源对象保持不变
- 移动: 新对象**直接接管**源对象内部的指针 / 句柄, 源对象被"掏空"成一个 valid-but-unspecified 的状态, 通常只剩析构能安全调用
- 拷贝是 O(资源大小), 移动一般是 O(1) — 只是几次指针赋值

## 一、基础用法和场景

### std::move 是什么 — 它是个 cast, 不是真的"移动"

`std::move` 这个名字非常容易误导. 它**没有移动任何东西**, 也不会修改对象, 它做的事情只是把一个左值**强制转换 (cast)** 成右值引用类型, 让重载决议优先选中接收 `T&&` 的那个版本

近似实现是这样的

```cpp
template <typename T>
typename std::remove_reference<T>::type&& move(T&& v) noexcept {
    return static_cast<typename std::remove_reference<T>::type&&>(v);
}
```

实际把"资源转移"做掉的是**移动构造函数 / 移动赋值运算符**. `std::move` 只是负责把对象**贴上一个"我可以被掏空"的标签**, 真正掏空它的是后面的构造或赋值

```cpp
Buffer a;
Buffer b = std::move(a); // 这里 std::move(a) 只是把 a 转成 Buffer&&,
                         // 真正的资源转移发生在 Buffer 的移动构造函数里
```

如果一个类型**没有定义**移动构造 / 移动赋值, `std::move` 会安静地退化成拷贝, 编译器不会报错. 这是初学最常踩的坑

### 移动构造函数 / 移动赋值运算符的形态

二者的签名固定为接收一个本类型的右值引用 `T&&`. 标准做法是: **把源对象的资源指针偷过来, 再把源对象那边置空**, 这样析构两次也不会重复释放

```cpp
struct Buffer {
    int *data;

    Buffer() : data { new int[2] {0, 1} } { }

    // 移动构造: 接管 other 的资源, 然后把 other 置空
    Buffer(Buffer&& other) noexcept : data { other.data } {
        other.data = nullptr;
    }

    // 移动赋值: 先释放自己的旧资源, 再接管 other 的资源, 最后把 other 置空
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

注意三个细节

- `noexcept` 几乎是必须的: `std::vector` 等容器在扩容时**只有在移动构造声明 noexcept 的前提下才会用移动**, 否则为了强异常保证会退回到拷贝
- 移动赋值要做 **self-assign 检查 + 释放旧资源**, 顺序不能反
- 析构函数要能容忍 `data == nullptr`, 因为被移动后的对象就是这种状态

### 编译器何时自动生成 / 不生成移动

编译器在满足下列条件时, 会**自动生成默认的移动构造和移动赋值**

- 用户没有自己声明拷贝构造 / 拷贝赋值 / 析构函数 / 移动构造 / 移动赋值

只要用户**显式声明了上面任何一个**, 移动操作就**不会**被自动生成 (这就是所谓"Rule of 5"提醒你: 一旦自定义其一, 其它五个相关函数都要重新审视)

```cpp
struct Foo {
    std::vector<int> v;
    // 没声明任何特殊成员 -> 编译器自动生成移动构造/赋值, 直接转发给 v
};

struct Bar {
    std::vector<int> v;
    ~Bar() { /* 任何自定义内容 */ }
    // 自定义析构 -> 移动构造/赋值不会自动生成, 拷贝时只能走深拷贝
};
```

如果默认的逐成员移动语义已经够用 (比如成员都是 `std::vector` / `std::unique_ptr` 这种本身已支持移动的类型), 不要写自己的. 想强制要求时可以用 `= default` 显式声明

```cpp
struct Bar {
    std::vector<int> v;
    ~Bar() { }
    Bar(Bar&&) = default;
    Bar& operator=(Bar&&) = default;
};
```

### 资源所有权转移的实际意义

回到开头那个例子, `Buffer` 持有一块堆缓冲. 没有移动语义时, 走 `process(Buffer())` 这条链路要发生多次"分配 + 拷贝 + 析构"

```cpp
Buffer process(Buffer buff) {  // 入参一次构造
    return buff;               // 返回值一次构造
}

Buffer b = process(Buffer());  // 临时实参又一次构造
```

加上移动构造之后, 临时对象 / 局部变量在被消耗的位置自动走 `Buffer&&` 重载, 整条链路只做一次 `new int[2]`, 所有中间对象共享同一块缓冲, 直到最后一个对象析构时才 `delete[]` 一次. 这正是练习 `05-move-semantics-0.cpp` 想让你**通过编译器输出亲眼看到**的现象

同样的思路推广到任何"持有外部资源"的类型: `std::unique_ptr`, 文件句柄包装, 网络连接 RAII, 大块图像 / 音频缓冲, 都靠移动语义把"传递成本"压到接近 0

## 二、注意事项

### 被移动后的对象处于 "valid-but-unspecified" 状态

标准对一个被移动后的对象只保证两点

- **析构函数能安全调用**
- 满足类型不变量的最低要求 (这个由你的实现决定)

但**不保证**它还有"原来的内容"或"任何特定状态". 在练习 `05-move-semantics-2.cpp` 里你会看到, 被移动后 `b1.data_ptr() == nullptr` — 这是**因为我们的实现**主动把它置空了, 不是语言强行规定. 一旦移动结束, 不要再去**读取**被移动对象的内容, 只允许**赋新值**或**让它析构**

```cpp
Buffer b1;
Buffer b2 = std::move(b1);
// b1 现在是 valid-but-unspecified 状态:
b1.data_ptr();  // 不要这样用 b1 的"业务数据"
b1 = Buffer();  // ok: 重新赋值
// 离开作用域时 b1 析构也是安全的
```

### "Rule of 0 / 3 / 5"

- **Rule of 0**: 优先把资源管理交给已经支持移动的标准类型 (`std::vector`, `std::unique_ptr`, `std::string`...), 自己一个特殊成员都不写, 编译器会替你处理好一切
- **Rule of 3** (C++98): 一旦自定义了**析构 / 拷贝构造 / 拷贝赋值**之一, 通常另两个也要自定义
- **Rule of 5** (C++11): 在 Rule of 3 的基础上, 再加上**移动构造 / 移动赋值** — 一旦你介入了资源管理, 这五个特殊成员要一起设计, 否则会出现"能拷贝不能移动"或"能移动但移动后状态错乱"的不一致情况

最稳妥的策略仍然是 Rule of 0: 让标准库类型来当资源持有者, 你的类只负责组合它们

### 不要滥用 std::move (尤其是返回局部对象时)

直接 `return localObj;` 时, 编译器有 **NRVO / RVO** (具名 / 非具名返回值优化), 能直接在调用方栈帧上构造对象, **连一次移动都不需要**. 主动写 `return std::move(localObj);` 反而会**抑制 NRVO**, 使得编译器只能走移动, 性能反而更差

```cpp
Buffer good() {
    Buffer b;
    return b;            // ok: 优先 NRVO, 次选移动
}

Buffer bad() {
    Buffer b;
    return std::move(b); // 不推荐: 抑制 NRVO, 强制走移动
}
```

类似的, **传值参数**已经是个新对象, 在函数内部把它进一步传出去时才考虑 `std::move`. 对**const 对象**用 `std::move` 也是无效的, `std::move` 出来的还是 const 右值引用, 重载决议会回退到拷贝构造

## 三、练习代码

### 练习代码主题

- 0 - [移动构造与触发时机 - 让 buff 传递只做一次资源分配](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/05-move-semantics-0.cpp)
- 1 - [移动赋值与触发时机 - 临时对象 / 中间对象 / 显式 std::move 三种场景](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/05-move-semantics-1.cpp)
- 2 - [移动的是资源而不是对象 - 对比对象地址和 data 指针](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/05-move-semantics-2.cpp)

### 练习代码自动检测命令

```
d2x checker move-semantics
d2x checker move-semantics-2
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
