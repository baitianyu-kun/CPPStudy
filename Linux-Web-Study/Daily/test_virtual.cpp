//
// Created by baitianyu on 5/13/24.
//
#include "iostream"
using namespace std;

class A
{
public:
    virtual ~A() { cout << "A" << endl; }
};

class B : public A
{
public:
    ~B() { cout << "B" << endl; }
};

int main()
{
    A* ptr = new B;
    delete ptr;
    return 0;
}