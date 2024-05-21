//
// Created by baitianyu on 5/21/24.
//
#include "assert.h"
#include "cstdio"
#include "cstdlib"
#include "cstring"
#include "fcntl.h"
#include "iostream"
#include "sys/stat.h"
#include "sys/types.h"
#include "unistd.h"

using namespace std;

int main(){

    int fd = open("test", O_RDONLY);
    assert(fd!=-1);
    while (1){
        char buf[1024]={0};
        int len = read(fd,buf, sizeof(buf));
        if (len==0){
            printf("write break......\n");
            break;
        }
        printf("recv buf: %s\n",buf);
    }
    close(fd);
    return 0;

}