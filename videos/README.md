# d2mcpp 解读视频/动画

## 动画代码

> Manim Community v0.18.1。manim 本体由 uv 装(纯 pip);pycairo/manimpango 的原生依赖
> —— xcb-free 的 cairo/pango 图形栈 —— 用 xlings 装进项目 subos,免去系统 gcc 的 xcb 坑。

```bash
# 1. 一次性:装图形栈 + uv 进项目 subos(cairo/pango 的 deps 自动拉全树)
xlings install cairo pango uv

# 2. 渲染:用启动器(已封装 PKG_CONFIG_PATH / uv 依赖 / pygments pin,免手输环境变量)
videos/render.sh cpp11/00-auto-and-decltype.py          # 默认 -ql 快预览
videos/render.sh cpp11/00-auto-and-decltype.py -qh      # 高清 1080p
```

<details>
<summary>启动器封装了什么 / 手动等价命令</summary>

`videos/render.sh <场景> [manim参数]` 等价于(在仓库根执行):

```bash
SR=$PWD/.xlings/subos/_/usr          # 项目匿名 subos —— xlings install 装在这, xlings-gcc 默认也搜它
PKG_CONFIG_PATH=$SR/lib/pkgconfig \
  uv run --with "manim==0.18.1" --with "pygments==2.17.2" \
  manim -ql videos/cpp11/00-auto-and-decltype.py
```

- `PKG_CONFIG_PATH` 指向项目 subos,让 pkg-config 命中 **xcb-free cairo**(而非宿主 apt 带 xcb 的)——仅首次/清缓存后编译 pycairo/manimpango 时需要,编好的 wheel 被 uv 缓存后可省。
- `pygments==2.17.2` 避开 manim 0.18.1 的 `Code` 渲染 `#CCC` bug。
- **兜底**(没装生态栈时,用系统 gcc 绕开 xlings-gcc 的 xcb 坑):
  ```bash
  CC=/usr/bin/gcc CXX=/usr/bin/g++ \
    uv run --with "manim==0.18.1" --with "pygments==2.17.2" \
    manim -ql videos/cpp11/00-auto-and-decltype.py
  ```
</details>

## 视频列表

| c++标准 | 特性 | 标题 | 练习代码/视频 | 备注 |
| --- | --- | --- | --- | --- |
| **引导** | `项目使用教程/引导` | hello mcpp | [docs](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/base/chapter_1.md) / [code](/dslings/hello-mcpp.cpp) / [video](https://www.bilibili.com/video/BV182MtzPEiX?p=2) | |
| **cpp11** | `00 - auto和decltype` | 类型自动推导 | [docs](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/00-auto-and-decltype.md) / [code](/videos/cpp11/00-auto-and-decltype.py) / [video](https://www.bilibili.com/video/BV1EzJs6HEf7) | |
| | | 练习讲解 | [code](/dslings/cpp11/00-auto-and-decltype-0.cpp) / [video](https://www.bilibili.com/video/BV1xkdYYUEyH) | |
| | | decltype注意事项 | [code](/dslings/cpp11/00-auto-and-decltype-4.cpp) / [video](https://www.bilibili.com/video/BV1KWoMYUEzW) | [补充](https://forum.d2learn.org/topic/82) |
| | `01 - default和delete` | 控制默认构造函数生成 | [code](/dslings/cpp11/01-default-and-delete-0.cpp) / [video](https://www.bilibili.com/video/BV1B35pz5EN2) | |
| | | 类型对象行为控制示例 | [code](/dslings/cpp11/01-default-and-delete-1.cpp) / [video](https://www.bilibili.com/video/BV1Vg5tznE8o) | |
| | `02 - override和final` | 重写显示意图和编译器检查 | [code](/dslings/cpp11/02-final-and-override-0.cpp) / [video](https://www.bilibili.com/video/BV1BdLJz6EKJ) | |
| | | 继承和重现限制 | [code](/dslings/cpp11/02-final-and-override-1.cpp) / [video](https://www.bilibili.com/video/BV1H1jAzTEYT) | |
| | `03 - 后置返回值类型` | 后置返回值类型基础用法 | [code](/dslings/cpp11/03-trailing-return-type.cpp) / [video](https://www.bilibili.com/video/BV1Ma5wzgE9h) | |
| | `04 - 右值引用` | 临时对象生命周期延长的可变性质 | [code](/dslings/cpp11/04-rvalue-references.cpp) / [video](https://www.bilibili.com/video/BV1vn5wzmEVk) | |
| | `05 - 移动语义` | 移动构造和触发时机 | [code](/dslings/cpp11/05-move-semantics-0.cpp) / [video](https://www.bilibili.com/video/BV19gj9zAERL) | |
| | | 移动赋值和触发时机 | [code](/dslings/cpp11/05-move-semantics-1.cpp) / [video](https://www.bilibili.com/video/BV1NDjRzREsY) | |
| | | 移动的不是对象而是资源 | [code](/dslings/cpp11/05-move-semantics-2.cpp) / [video](https://www.bilibili.com/video/BV1P9jRzXE3a) | |
| | `06 - 作用域枚举` | 传统枚举的问题 | [code](/dslings/cpp11/06-scoped-enums-0.cpp) / [video](https://www.bilibili.com/video/BV1fn7iz4EuR) | |
| | | 作用域枚举的基本用法 | [code](/dslings/cpp11/06-scoped-enums-1.cpp) / [video](https://www.bilibili.com/video/BV1fn7iz4EuR) | |
| | `07 - constexpr` | const和constexpr有什么不同 | [code](/dslings/cpp11/07-constexpr-0.cpp) / [video](https://www.bilibili.com/video/BV1LRMLzgE4w) | |
| | | 编译期计算应用示例 | [code](/dslings/cpp11/07-constexpr-1.cpp) / [video](https://www.bilibili.com/video/BV15CMEzLEuN) | |
| | `08 - literal-type` | 什么是字面值类型 | [code](/dslings/cpp11/08-literal-type-0.cpp) / [video](https://www.bilibili.com/video/BV1h23UzvEX6) | [编译期加密库示例](https://github.com/mcpp-community/honly/blob/main/honly_jiami.hpp) |
| | | 如何定义一个字面值类型 | [code](/dslings/cpp11/08-literal-type-1.cpp) / [video](https://www.bilibili.com/video/BV1h23UzvEX6) | |
| | `09 - 列表初始化` | 现代C++初始化风格 | [code](/dslings/cpp11/09-list-initialization-0.cpp) / [video](https://www.bilibili.com/video/BV1vKuQzkEo2) | |
| | | -> | [code1](/dslings/cpp11/09-list-initialization-1.cpp) / [code2](/dslings/cpp11/09-list-initialization-2.cpp) / [code3](/dslings/cpp11/09-list-initialization-3.cpp)  | |
| | `10 - 委托构造函数` | 委托构造基础用法和注意事项 | [code](/dslings/cpp11/10-delegating-constructors-0.cpp) / [video](https://www.bilibili.com/video/BV1zft3zSEER) | |
| | | -> | [code1](/dslings/cpp11/10-delegating-constructors-1.cpp) | |
| | `11 - 继承构造函数` | 继承构造函数的基础用法 | [code](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/11-inherited-constructors-0.cpp) / [video](https://www.bilibili.com/video/BV1bspBzFEEC) | |