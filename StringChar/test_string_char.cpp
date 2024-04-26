//
// Created by 白天宇 on 2024/4/26.
//
#include "iostream"
#include "string"
using namespace std;

int main(){

    string* str = new string("!231231231");
    cout<<*str<<"    "<<*(str+1);
    delete str;
    str = nullptr;
}