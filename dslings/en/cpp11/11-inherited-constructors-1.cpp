// d2mcpp: https://github.com/mcpp-community/d2mcpp
// license: Apache-2.0
// file: dslings/cpp11/11-inherited-constructors-1.cpp
//
// Exercise: cpp11 | 11 - inherited constructors | Inherited Constructors for Function Extension
//
// Tips: Based on compiler output and error prompts, implement the required Student helper test class
//
// Docs:
//   - https://en.cppreference.com/w/cpp/language/using_declaration.html#Inheriting_constructors
//   - https://mcpp-community.github.io/d2mcpp/cpp11/11-inherited-constructors.html
//
// Auto-Checker command:
//
//   d2x checker inherited-constructors
//

#include <d2x/common.hpp>

#include <iostream>
#include <string>

class Student { // Do not directly modify the code in the Student class
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

    D2X_YOUR_ANSWER // Implement Student's helper test class

    std::string to_string() const {
        return "{" + id + ", " + name + ", " + std::to_string(age) + ", " + std::to_string(score) + "}";
    }
};

// Test requirements:
//  score_valid: [0 ~ 100]
//  age_valid: (0 ~ 200]
//  to_string: {id, name, age, score}

int main() { // Do not directly modify the code in the main function

    { // Basic test
        StudentTest studentTest;

        d2x_assert(studentTest.age_valid() == true);
        d2x_assert(studentTest.score_valid() == true);
        d2x_assert(studentTest.to_string() == "{001, 张三, 18, 0}");
    }

    { // Boundary test
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

    D2X_WAIT

    return 0;
}