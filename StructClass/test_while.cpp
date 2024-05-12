//
// Created by 白天宇 on 2024/5/10.
//
#include "iostream"
using namespace std;

int returnN(){
    return 12;
}

int main(){
    int res = 0;
    // while 循环的判断条件是 (res = returnN()) == 12。
    // 这意味着在每次循环迭代时，将调用函数 returnN() 并将其返回值赋给变量 res。
    // 然后，判断 res 的值是否等于 12。
    while ((res =  returnN()) == 12){
        cout<<res<<endl;
        break;
    }
}