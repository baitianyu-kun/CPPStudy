//
// Created by 白天宇 on 2024/5/28.
//
#include "iostream"
#include "algorithm"



int main(){
    std::string res="hello2";
    std::string str1 = "hello";
    //
    // Just use ::toupper instead of std::toupper. That is,
    // toupper defined in the global namespace, instead of the one defined in std namespace.
    // std::transform(s.begin(), s.end(), std::back_inserter(out), ::toupper);
    transform(str1.begin(),str1.end(),res.begin(), ::toupper);
    std::cout<<res<<std::endl;

}