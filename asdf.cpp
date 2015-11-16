//
// Created by j0sh on 11/5/15.
//

#include "asdf.h"
#include <iostream>
#include "image_utils.h"

class A {
protected:
    int b;
public:
    int d;
};

class B : public A {
};

class C : public B {
public:
    void setB(int n) { b = n; };

    int getB() { return b; };
};

int main() {
    using namespace std;
    C asdf;
    asdf.setB(5);
    cout << asdf.getB() << endl;
    asdf.d = 6;
    cout << asdf.d << endl;
//    RGB test;
//    RGB *test_ptr = &test;
//    test.r = 3;
//    cout << (int) test.r << endl;
//    cout << (int) test_ptr->r << endl;
//    cout << test_ptr << endl;
//    (*test_ptr).g = 5;
//    *test_ptr.g = 5; // does not work because operator precedence
}
