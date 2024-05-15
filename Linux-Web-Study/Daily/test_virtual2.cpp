//
// Created by baitianyu on 5/14/24.
//
#include "iostream"
using namespace std;

class A{
public:
    A(){

    }
    ~A(){
        cout<<"~A"<<endl;
    }
};

class B: public A{
public:
    B(){

    }
    ~B(){
        cout<<"~B"<<endl;
    }
};

int main(){

    B *b1 = new B;
    delete b1;
    return 0;

}