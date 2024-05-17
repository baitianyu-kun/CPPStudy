//
// Created by 白天宇 on 2024/5/15.
//
#include "iostream"

using namespace std;

void fun1(int* val1, int* val2) {
    cout << *val1 + *val2 << endl;
}

int main() {


    // 这里的类型后面的变量可写可不写
    function<void(int* va232323l1, int* va23232323l2)> funptr = nullptr;
    int n1 = 1;
    int n2 = 2;

    funptr = fun1;
    funptr(&n1, &n2);

}