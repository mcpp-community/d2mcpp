<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./03-trailing-return-type.html
[English]: ../en/cpp11/03-trailing-return-type.html

# 尾置返回类型 - trailing return type

尾置返回类型是 C++11 引入的一种新的函数声明语法 `auto func(...) -> ReturnType`, 把返回类型从函数名前面挪到参数列表之后. 它解决了 "返回类型依赖参数" 在传统语法下根本写不出来的问题, 同时也是 lambda 显式指定返回类型的统一语法形式

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/function) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/03-trailing-return-type.md) | [视频解读]() | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/03-trailing-return-type.cpp) |  |


**为什么引入?**

- 传统语法下, 返回类型写在函数名前面, 此时**参数还没有出现在作用域中**, 无法用参数去推导返回类型
- 模板编程中经常需要 "返回类型 = 某个表达式的类型", 没有这种语法, decltype 推导依赖参数的返回类型几乎不可写
- 让函数签名的形态更统一: lambda、普通函数、模板函数都能用同一种 `auto ... -> T` 形式

**和传统返回类型语法有什么区别?**

- 传统写法: `ReturnType func(Args...)`, 返回类型在最前面
- 尾置写法: `auto func(Args...) -> ReturnType`, 用 auto 占位, 真正的返回类型放在 `->` 之后
- 两者大多数情况下完全等价, 但只有尾置写法能在 `->` 之后**引用参数名**, 这是它真正不可替代的能力

## 一、基础用法和场景

### 基本语法 - auto + ->

把返回类型从函数名前移到参数列表之后, 中间用 `->` 连接. 函数名前用 `auto` 占位, 表示 "返回类型在后面写"

```cpp
// 传统写法
int add(double a, int b) {
    return a + b;
}

// 尾置返回类型写法 - 等价
auto add(double a, int b) -> int {
    return a + b;
}
```

这两种写法在编译结果上完全一致. 单看普通函数, 尾置写法只是**风格差异**, 并没有带来新的能力

### 配合 decltype 推导依赖参数的返回类型

尾置返回类型真正的杀手锏在模板里. 写一个泛型 add, 想让它支持 int、double、Point 等任意可相加的类型, 返回值是 `a + b` 的类型, 但具体是什么取决于 T1 和 T2

传统写法**写不出来**, 因为返回类型出现时, a 和 b 还没在作用域里

```cpp
// 写不出来 - 此处 a, b 还未声明
decltype(a + b) add(T1 a, T2 b);
```

尾置语法把返回类型移到参数之后, 此时 a、b 已经在作用域中, 就可以用 decltype 直接推导

```cpp
template<typename T1, typename T2>
auto add(const T1 &a, const T2 &b) -> decltype(a + b) {
    return a + b;
}

add(1, 2);     // 返回 int
add(1.1, 2);   // 返回 double
add(1, 2.1);   // 返回 double
```

这是**尾置返回类型唯一不可被替代的核心场景**: 让返回类型表达式可以引用参数名

> 在 C++14 之后, 普通函数可以直接写 `auto add(...)` 让编译器从 return 语句推导, 大多数情况不再需要写 `-> decltype(a + b)`. 但 C++11 仍然必须用尾置语法

### lambda 显式指定返回类型

lambda 没有名字, 也就没有 "返回类型写哪里" 的选择 - 它从一开始就只能用尾置语法

```cpp
auto add = [](double a, double b) -> int {
    return a + b;  // 显式截断为 int
};

add(1.1, 2.1); // 3, 不是 3.2
```

不写 `-> int` 时, lambda 会自己推导返回类型为 double; 显式标注后, 返回值会被转换成指定的类型. 这是 lambda 控制返回类型的标准做法

### 嵌套类型 / 成员类型作为返回值

当返回类型是某个类的内嵌类型时, 传统写法需要在前面写完整限定 `typename Class::Inner`, 模板里还要加 typename 关键字, 又长又啰嗦

```cpp
template<typename T>
typename std::vector<T>::iterator find_first(std::vector<T> &v, T x);
```

尾置写法可以让函数名先出现, 在 `->` 之后写返回类型. 在某些类成员函数定义中, 可以省去重复的类名前缀

```cpp
struct Box {
    struct Inner { /* ... */ };

    auto make() -> Inner;  // 返回类型只写 Inner
};

auto Box::make() -> Inner {  // 此时已经在 Box 的作用域内
    return Inner{};
}
```

## 二、注意事项

### auto 在尾置语法中只是占位符

尾置写法里的 auto **不是类型推导**, 它只是一个语法占位符, 真正的类型由 `->` 之后给出. 这点和 C++14 的 `auto func() { return ...; }` 不同, 后者才是真正让编译器去推导

```cpp
auto add(double a, int b) -> int {  // C++11: 返回类型由 -> int 显式给出
    return a + b;
}

auto add(double a, int b) {         // C++14: 返回类型由编译器从 return 语句推导
    return a + b;
}
```

C++11 中如果只写 `auto func()` 而省略 `->`, 是编译错误

### 何时用尾置, 何时用传统

不要把所有函数都改成尾置写法. 经验法则:

- 返回类型**依赖参数** (decltype(a + b) 之类) -> 必须用尾置
- lambda 想显式指定返回类型 -> 必须用尾置
- 类成员函数返回内嵌类型, 想省掉 `Class::` 前缀 -> 尾置更简洁
- 普通函数, 返回类型简单 (int / void / std::string) -> 传统写法可读性更好, 没必要换

### 不是所有 "auto func" 写法都是尾置返回类型

`auto func() -> int` (C++11 尾置) 和 `auto func() { return 1; }` (C++14 返回类型推导) 写起来都以 auto 开头, 但语义完全不同

- 前者: auto 是占位, 真实类型由 `->` 给出, 编译器**不需要看函数体**
- 后者: 编译器必须看 return 语句才能推导出返回类型, 函数声明和定义就不能完全分离 (头文件里只放声明就拿不到返回类型)

在写需要在头文件中声明、源文件中实现的函数时, 这个区别会直接影响是否能正常编译

### 返回类型仍然受常规规则约束

尾置返回类型只是**位置变了**, 类型本身的规则没变:

- 不能返回函数 / 数组 (要返回函数指针 / 数组指针)
- decltype 推导出来的引用 / const 限定都会保留
- 派生类重写虚函数时, 返回类型仍要满足协变规则 (covariant return)

```cpp
int arr[3];
// auto func() -> int[3];  // 错误: 不能返回数组
auto func() -> int(*)[3];   // ok: 返回数组指针
```

## 三、练习代码

### 练习代码主题

- 0 - [熟悉尾置返回类型 - 普通函数 / 模板 / lambda 三种形态](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/03-trailing-return-type.cpp)

### 练习代码自动检测命令

```
d2x checker trailing-return-type
```

## 四、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/d2learn/xlings)
