//
// Created by 白天宇 on 2024/5/24.
//
#include "iostream"
using namespace std;

int strcmp(const char *p, const char *q)
{
    while(*p && *p == *q)
        p++, q++;
    return (unsigned char)*p - (unsigned char)*q;
}

int main(){

    cout<<strcmp("b","b");

}