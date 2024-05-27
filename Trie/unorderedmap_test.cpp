//
// Created by BAITIANYU on 2024/5/27.
//
#include "iostream"
#include "unordered_map"
using namespace std;

int main() {

    unordered_map<char,string> maps;
    maps.emplace('h',"hello");
    cout<<maps.at('h')<<endl;
    auto iter = maps.find('j');
    if (iter==maps.end()) {
        cout<<"nullptr4"<<endl;
    }

}
