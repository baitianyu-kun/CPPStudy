//
// Created by 白天宇 on 2024/4/23.
//
#include "../include/register.h"
using namespace reflect;

int main(){

    Register rg;
    Factory* ft = rg.registerFunction();
    // RTTI
    // RTTI(run-time type identification), 运行时类型标识.
    // 可以在Runtime拿到各种数据的一些简单类型信息, 如名字等等.
    // RTTI在c++的两个应用为 typeid 和 dynamic_cast
    // 可以看到在编译期没有虚函数表(多态类型)的不能进行dynamic_cast

    // dynamic_cast 常用于从多态编程基类指针向派生类指针的向下类型转换，
    // 运行时转换，进行了类型检查，速度比较慢，其会在多态时候一个个比较虚函数表。
    // static_case 从派生向基类转换即向上转换是安全的，
    // 向下转换不安全，因为没有进行类型匹配，但是速度快
    Reader *prd = dynamic_cast<Reader *>(ft->Create("Reader"));
    // 如果dynamic cast解析出错的话，就会返回空指针
    Reader *prd2 = dynamic_cast<Reader *>(ft->Create("Writer"));
    if (prd2== nullptr){
        cout<<"ptd2 is null"<<endl;
    }
    Writer *prd3 = dynamic_cast<Writer *>(ft->Create("Writer"));
    prd->Process();
    prd3->Process();
}