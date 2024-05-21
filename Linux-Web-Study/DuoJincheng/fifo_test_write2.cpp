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

    // 1.判断有名管道文件是否存在
    int ret = access("fifo1", F_OK);
    if(ret == -1) {
        // 文件不存在
        printf("管道不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo1", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    ret = access("fifo2", F_OK);
    if(ret == -1) {
        // 文件不存在
        printf("管道不存在，创建对应的有名管道\n");
        ret = mkfifo("fifo2", 0664);
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    // 2.以只读的方式打开管道fifo1
    int fdr = open("fifo1", O_RDONLY);
    printf("打开管道fifo1成功，等待读取...\n");
    // 3.以只写的方式打开管道fifo2
    int fdw = open("fifo2", O_WRONLY);
    printf("打开管道fifo2成功，等待写入...\n");

//    dup2(fdw, STDOUT_FILENO);
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