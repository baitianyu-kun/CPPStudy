//
// Created by 白天宇 on 2024/5/15.
//
#include "iostream"
#include "unordered_map"
using namespace std;

static unordered_map<string,string> mymap;

struct MyStruct{
    MyStruct(string &str1,string &str2){
        mymap.emplace(str1,str2);
    }
};

int main(){

    string str1 = "hello1";
    string str2 = "hello2";
    string str3 = "hello3";
    string str4 = "hello4";
    const MyStruct mine ={str1,str2};

    const MyStruct mine2 =  {str3,str4};

    for (auto &item:mymap) {
        cout<<item.first<<" "<<item.second<<endl;
    }

}