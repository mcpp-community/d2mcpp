<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./17-pod.md
[English]: ../en/cpp11/17-pod.md

# POD（Plain Old Data）

在 C 语言中，常见大量仅包含数据成员、没有构造或析构语义、可以被视为按位存储的结构体。
C++ 早期标准为了描述这类“行为上接近 C struct 的类型”，引入了 POD（Plain Old Data） 这一概念。

| 书籍 | 视频 | 代码 | 交流 |
| --- | --- | --- | --- |
| [cppreference-PODType](https://cppreference.com/w/cpp/named_req/PODType.html) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/17-pod-type.md) | [视频解读]() | [练习代码]() | [论坛讨论](https://forum.d2learn.org/category/20) |

> 注意：从 C++20 开始，标准中的 “PODType” 概念已被标记为**弃用**。标准库更倾向于使用更细化的类别，如 `TrivialType`、`StandardLayoutType`、`ScalarType` 等来描述相关需求。

**为什么引入 POD**

- C++ 需要兼容大量已有的 C 风格数据结构；
- 早期标准希望提供一种“近似 C struct”的类型分类，用于描述：
  - 对象是否具有简单、可预测的内存布局；
  - 对象是否不涉及复杂的构造、析构语义；
- 在当时，尚未引入细粒度的类型特征（type traits），POD 被用作一种**方便但不精确的标签**。

**POD 与其他类型类别的关系**

- 所有**标量类型（ScalarType）**都是 POD，例如：
  - 内置算术类型：`int`、`double`、`char` 等；
  - 枚举类型 `enum`；
  - 各种指针类型。
- 对于类类型，标准引入以下概念：
  - **平凡类型（TrivialType）**：所有特殊成员函数（构造、拷贝、移动、析构）都是平凡的（编译器自动生成或 `=default`，且不涉及虚函数等）；
  - **标准布局类型（StandardLayoutType）**：对象的内存布局规则简单、可预测（例如继承关系单一、访问控制集中等）。
- 一个类是 **POD 类**，当且仅当：
  - 它是**平凡类型**，并且
  - 它是**标准布局类型**。

例如
```cpp
struct A {
  int x;
  double y;
};              // POD：仅包含内置类型成员，且未定义任何特殊成员函数

struct B {
  A a;
  int z;
};              // 仍然是 POD：所有成员均为 POD 类型

struct C {
  virtual void foo();
  int x;
};              // 不是 POD：含有虚函数，破坏了平凡性和标准布局

struct D {
  int x;
private:
  int y;
};              // 不是 POD：私有与公有成员混排，破坏了标准布局（除非所有成员访问控制相同）
```


## 一、基础用法与典型场景

**与 C 接口交互**

> 使用 POD 结构体描述 C 接口所需的二进制数据布局，可直接按字节读写。

```cpp
struct Packet {
  std::uint32_t len;
  std::uint16_t type;
  std::uint16_t flags;
};  // 典型的 POD 结构体

int main() {
  Packet p{};
  // 假设 fd 是已打开的文件或 socket
  read(fd, &p, sizeof(p));
  write(fd, &p, sizeof(p));
}
```
这里用到的是它的 standardLayout 性质。

**简单的内存快照**

> POD 类型可被视为“字节数组”进行拷贝，在同一平台与相同编译设置下通常是安全的。

```cpp
struct Point {
  float x;
  float y;
};  // POD

void copy_points(const Point* src, Point* dst, std::size_t n) {
  std::memcpy(dst, src, n * sizeof(Point));  // 按字节拷贝
}
```

**与类型特征（type traits）配合**

> 早期代码常使用 `std::is_pod` 约束模板参数；现代 C++ 更推荐使用更细化的 traits。

```cpp
template <typename T>
void pod_only_copy(const T& src, T& dst) {
  static_assert(std::is_pod<T>::value, "T must be POD");
  std::memcpy(&dst, &src, sizeof(T));
}
```

在新代码中，建议根据具体需求使用以下约束之一：
- `std::is_trivially_copyable<T>`（可平凡拷贝）
- `std::is_standard_layout<T>`（标准布局）
- `std::is_scalar<T>`（标量类型）

## 二、注意事项
**POD 概念在 C++20 中已被弃用**

- 自 C++20 起，标准将 “PODType” 标记为 **deprecated**；
- 新增或修改接口时，建议直接使用更精确的特征检查：
  - `std::is_trivial<T>` / `std::is_trivially_copyable<T>`
  - `std::is_standard_layout<T>`
  - `std::is_scalar<T>`

**POD 不保证跨平台/跨编译设置的二进制兼容性**

- 不同平台、编译器或编译选项可能导致不同的内存对齐与填充（padding）；
- 若将 POD 二进制数据持久化（如存入文件或网络传输），并在另一环境中直接 `reinterpret_cast`，可能会因字节序、对齐方式等差异而导致错误。

**过度使用 POD 会影响程序灵活性**

- 许多现代 C++ 类型（如 `std::string`、`std::vector`）并非 POD，但它们提供了更安全、更高效的抽象；
- 通常仅在需要与底层 C 接口交互、进行二进制序列化或直接内存操作的模块中，才需要考虑 POD 或类似约束；其他场景应优先使用现代 C++ 的抽象与安全性特性。

## 三、练习代码

### 练习代码主题

- 0 - 使用类型特征判断 POD / trivial / standard layout（`17-pod-type-0.cpp`）
- 1 - 模拟按字节拷贝 POD 结构体，体会其行为（`17-pod-type-1.cpp`）
- 2 - 为 C 接口传入合适的 POD 类型数据（`17-pod-type-2.cpp`）

### 练习代码自动检测命令

```bash
d2x checker pod-type
```

## 四、其他资源

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp 教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具 - xlings](https://github.com/d2learn/xlings)

