//
// Created by baitianyu on 5/13/24.
//
#include "iostream"
using namespace std;

class A{
private:
    string name = "hello";
public:
    void get_nameA(){
        cout<<"A_name: "<<name<<endl;
    }
    A(){}
    virtual ~A(){}
};

class B:virtual public A{
public:
    void get_nameB(){
        get_nameA();
    }
};

class C:virtual public A{
public:
    void get_nameC(){
        get_nameA();
    }
};

class D:public B,public C{
public:
    void get_nameD(){
        get_nameA();
    }
};

int main(){
    A* a = new D;
    a->get_nameA();
}