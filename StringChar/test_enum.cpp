//
// Created by 白天宇 on 2024/5/12.
//
#include "iostream"
#include "type_traits"

using namespace std;

enum MyEnum {
    kRed = 0,
    kGreen = 1,
    kBlue = 15
};

template<class T>
void print() {
    MyEnum red = MyEnum::kRed;
    MyEnum green = MyEnum::kGreen;
    MyEnum blue = MyEnum::kBlue;

    // typename std::underlying_type<MyEnum>::type: typename用来标记后面的为类型，而不是underlying_type中的一个静态成员变量，不加也行
    cout<<static_cast<typename std::underlying_type<MyEnum>::type>(blue)<<endl;
    cout<<__PRETTY_FUNCTION__ ;
}

int main() {

    print<MyEnum>();

}