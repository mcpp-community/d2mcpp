// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// reference solution for: dslings/cpp11/11-inherited-constructors-1.cpp
//
// 用途: 仅给 CI 与维护者参考使用，不是教程入口。
// 教程练习入口: dslings/cpp11/11-inherited-constructors-1.cpp
//

#include <d2x/cpp/common.hpp>

#include <iostream>
#include <string>

class Student { // 不要直接修改 Student 类中的代码
protected:
    //...
    int score;
public:
    std::string id;
    std::string name;
    int age;

    Student() : id("001"), name("张三"), age(18), score(0) {
        std::cout << "Student::Student()" << std::endl;
    }

    Student(std::string id, std::string name, int age = 18)
        : id(id), name(name), age(age), score(0) {
        std::cout << "Student::Student(string, string, int)" << std::endl;
    }

    void set_score(int s) { score = s; }
    void set_age(int a) { age = a; }
};

class StudentTest : public Student {
public:
    // 继承 Student 的所有构造函数
    using Student::Student;

    bool age_valid() const {
        return age > 0 && age <= 200;
    }

    bool score_valid() const {
        return score >= 0 && score <= 100;
    }

    std::string to_string() const {
        return "{" + id + ", " + name + ", " + std::to_string(age) + ", " + std::to_string(score) + "}";
    }
};

// 测试要求:
//  score_valid: [0 ~ 100]
//  age_valid: (0 ~ 200]
//  to_string: {id, name, age, score}

int main() { // 不要直接修改 main 函数中的代码

    { // 基础测试
        StudentTest studentTest;

        d2x_assert(studentTest.age_valid() == true);
        d2x_assert(studentTest.score_valid() == true);
        d2x_assert(studentTest.to_string() == "{001, 张三, 18, 0}");
    }

    { // 边界测试
        StudentTest studentTest("002", "张三", 201);

        d2x_assert(studentTest.score_valid() == true);
        d2x_assert(studentTest.age_valid() == false);

        studentTest.set_score(101);
        studentTest.set_age(200);
        d2x_assert(studentTest.score_valid() == false);
        d2x_assert(studentTest.age_valid() == true);

        studentTest.set_score(0);
        studentTest.set_age(1);
        d2x_assert(studentTest.score_valid() == true);
        d2x_assert(studentTest.age_valid() == true);

        studentTest.set_score(-1);
        studentTest.set_age(0);
        d2x_assert(studentTest.score_valid() == false);
        d2x_assert(studentTest.age_valid() == false);

    }

    return 0;
}
