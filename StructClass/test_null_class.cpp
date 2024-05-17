//
// Created by 白天宇 on 2024/5/17.
//
#include "iostream"

using namespace std;

class Test{
public:
    void hello(){
        cout<<"hello"<<endl;
    }
};

int main(){

    // 这段代码可以运行是因为在C++中，对空指针进行成员访问是一种合法的行为，但这实际上是一种未定义的行为，可能导致程序崩溃。
    //在你的代码中，你创建了一个名为Test的类，其中包含一个名为hello的成员函数。在main函数中，你声明了一个名为p的指向Test对象的指针，
    // 并将其初始化为nullptr，即空指针。然后你尝试通过空指针p来调用hello函数。
    //这段代码在编译时不会出错，因为C++允许对空指针进行成员访问。然而，由于p是空指针，实际上没有指向有效的Test对象，因此在运行时会导致未定义的行为。
    // 这可能会导致程序崩溃、段错误或其他异常行为。
    Test * p = nullptr;
    p->hello();
    return 0;
}