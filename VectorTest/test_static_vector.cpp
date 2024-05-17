//
// Created by 白天宇 on 2024/5/17.
//
#include "iostream"
#include "vector"

using namespace std;

// 证明了虽然变量是静态的，但是可以调用其他指针
// 也就是说vector虽然是个静态对象，但是其中有allocator指针去堆上分配内存，allocator也是封装了new相当于
// 1. vector 在函数中定义(vector<int > arr；)， 对象是在栈上(&arr 是栈内存地址)， 数据在堆上(& arr[0]是堆内存地址)。
// 2. vector 在函数中new 出（vector<int>* temp = new vector<int>(10,1);）， 指针在栈上（&temp）， 对象地址在堆上（temp ）， 数据在堆上 （数据  (*temp ).at(i);   or  (*temp )[i]; 取址） 。
// 其中at（i）会做越界检查，耗时多。
// 总结： vector 的数据总是在堆上的，但对象的地址根据定义的方式不同可能在堆上，也可能在栈上。
// 对于std::vector<T> vec;vec在栈上（stack），而其中的元素T保存在堆上（heap）；
// 对于std::vector<T>* Vec = new std::vector<T>();vec和其中的元素T都保存在堆上；
// 对于std::vector<T*> vec;vec在栈上（stack），而其中的元素T保存在堆上（heap）；和第一种情况类似。
class A{
public:
    int i =0;
    A(int j){
        i = j;
    }
};

class B{
public:
    A *anext;
    B(){
        anext = new A(12);
        cout<<"a next created: "<<anext->i<<endl;
    }
    ~B(){
        delete anext;
        cout<<"a next destroyed"<<endl;
    }

};

int main() {

    static B b;

}