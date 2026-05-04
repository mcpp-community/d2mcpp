<div align=right>

  🌎 [中文] | [English]
</div>

[中文]: ./10-delegating-constructors.html
[English]: ../en/cpp11/10-delegating-constructors.html

# 委托构造函数

委托构造是C++11中引入的语法糖, 通过简单的语法, 可以在不影响性能的情况下, 来避免过多重复代码的编写, 实现构造逻辑复用

| Book | Video | Code | X |
| --- | --- | --- | --- |
| [cppreference](https://en.cppreference.com/w/cpp/language/initializer_list.html#Delegating_constructor) / [markdown](https://github.com/mcpp-community/d2mcpp/blob/main/book/src/cpp11/10-delegating-constructors.md) | [视频解读](https://www.bilibili.com/video/BV1zft3zSEER) | [练习代码](https://github.com/mcpp-community/d2mcpp/blob/main/dslings/cpp11/10-delegating-constructors-0.cpp)  |  |

**为什么引入?**

- 构造函数重载中, 避免重复代码的编写
- 方便代码的维护

## 一、基础用法和场景

### 复用构造逻辑

当一个类需要编写重载的构造函数时, 很容易造成大量的重复代码, 例如:

```cpp
class Account {
    string id;
    string name;
    string coin;
public:

    Account(string id_) {
        id = id_;
        name = "momo";
        coin = "0元";
    }

    Account(string id_, string name_) {
        id = id_;
        name = name_;
        coin = "0元";
    }

    Account(string id_, string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";
    }
};
```

这里3个构造函数中的初始化代码, 很明显是重复了(实际的初始化可能要更复杂)。 有了**委托构造**的支持后, 通过**在构造函数成员初始化列表**的位置以` : Account(xxx) `的形式来委托其他更加完整实现的构造函数进行构造, 这样就可以只保留一份代码

```cpp
class Account {
    string id;
    string name;
    string coin;
public:

    Account(string id_) : Account(id_, "momo") { }

    Account(string id_, string name_) : Account(id_, name_, 0) { }

    Account(string id_, string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";
    }
};
```

上面的两个构造函数, 通过委托构造的方式, 最后都会转发到`Account(string id_, string name_, int coin_)`

### 为什么更方便维护?

可以假设, 如果上面货币的单位或名称需要修改时, 重复的代码实现不仅没有遵循复用原则, 而且修改构造逻辑时也要重复多次的修改, 提高了维护成本

而通过委托构造的方式, 把构造逻辑放到了一个地方, 这样修改和维护时也变的更加方便

例如, 我们需要把`元`改成`原石`时, 只要修改一次即可

```cpp
class Account {
    // ...
    Account(string id_, string name_, int coin_) {
        //...
        //coin = std::to_string(coin_) + "元";
        coin = std::to_string(coin_) + "原石";
    }
};
```

### 和封装成一个init函数的区别

一些朋友可能会想到, 如果把构造逻辑写成一个`init`函数, 不就是也可以实现代码复用的效果吗? 为什么还要搞一个新的写法, 作为特性添加到标准中. 是不是有点多余并且让C++变的更加复杂了

```cpp
class Account {
    // ...

    init(string id_, string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";
    }

public:

    Account(string id_) { init(id_, "momo", 0); }

    Account(string id_, string name_) { init(id_, name_, 0); }

    Account(string id_, string name_, int coin_) {
        init(id_, name_, coin_);
    }
};
```

实际, 从性能角度考虑。大多数时候, 单独封装一个`init`函数的性能是低于**委托构造**的。因为成员的构造, 一般会经历两个阶段:

- 第一步: 执行 默认初始化 或 成员初始化列表
- 第二步: 运行构造函数体中的构造逻辑

```cpp
class Account {
    // ...
public:

    Account(string id_, string name_, int coin_)
        /* : 1 - 成员初始化列表 */
    {
        // 2 - 执行构造函数的函数体
        init(id_, name_, coin_);
    }
};
```

这就导致使用init函数, 实际上成员被"初始化"了两次, 而**委托构造**可以通过**成员初始化列表**来避免这个问题

```cpp
class Account {
    // ...
public:

    Account(string id_, string name_, int coin_)
        : id { id_ }, name { name_ }, coin { std::to_string(coin_) + "元" }
    {
        // ...
    }
};
```

## 二、注意事项

### 临时对象误会

在一些不使用委托构造的场景中, 一个构造函数体中调用另外一个构造函数, 他实际只是创建了一个临时对象

- 调用普通函数`init`: 初始化的是本对象的成员
- 调用另外一个构造函数: 在本对象外, 创建了一个新的临时对象

```cpp
class Account {
    // ...
public:

    Account(string id_, string name_) {
        Account(id_, name_, 0); // 创建的是临时对象
        // init(id_, name_, 0);
        // this->Account(id_, name_, 0); // error
    }

    Account(string id_, string name_, int coin_) {
        id = id_;
        name = name_;
        coin = std::to_string(coin_) + "元";
    }
};

```

### 不能重复初始化

当使用**委托构造**时, 就不能使用初始化列表去初始化其他成员, 这样的限制可以避免重复的初始化, 保证了数据成员只会被初始化一次

例如, 如果下面的语法被允许 `coin` 将会被初始化多次且可能会造成歧义

```cpp
class Account {
    // ...
public:

    Account(string id_)
        : Account(id_, "momo"), coin { "0元" } // error
    {

    }

};
```

## 三、其他

- [交流讨论](https://forum.d2learn.org/category/20)
- [d2mcpp教程仓库](https://github.com/mcpp-community/d2mcpp)
- [教程视频列表](https://space.bilibili.com/65858958/lists/5208246)
- [教程支持工具-xlings](https://github.com/openxlings/xlings)