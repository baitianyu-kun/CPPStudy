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
int main() {

    // 2.以只写的方式打开管道fifo1
    int fdw = open("fifo1", O_WRONLY);
    printf("打开管道fifo1成功，等待写入...\n");
    // 3.以只读的方式打开管道fifo2
    int fdr = open("fifo2", O_RDONLY);
    printf("打开管道fifo2成功，等待读取...\n");

    tee(STDOUT_FILENO,fdw,32768, SPLICE_F_NONBLOCK);
    string str;
    char buf[1024] = {0};
    while (getline(cin, str)) {
        cout << str << endl;//通过标准输出将读入的数据打印出来
        memset(buf,0,1024);
        int len = read(fdr, buf, 1024);
        printf("recv buf: %s\n", buf);
    }
    close(fdw);
    close(fdr);
    return 0;
}