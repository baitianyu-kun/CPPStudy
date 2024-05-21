//
// Created by baitianyu on 5/20/24.
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
    // 有名管道的注意事项：
    //        1.一个为只读而打开一个管道的进程会阻塞，直到另外一个进程为只写打开管道
    //        2.一个为只写而打开一个管道的进程会阻塞，直到另外一个进程为只读打开管道
    //
    //    读管道：
    //        管道中有数据，read返回实际读到的字节数
    //        管道中无数据：
    //            管道写端被全部关闭，read返回0，（相当于读到文件末尾）
    //            写端没有全部被关闭，read阻塞等待
    //
    //    写管道：
    //        管道读端被全部关闭，进行异常终止（收到一个SIGPIPE信号）
    //        管道读端没有全部关闭：
    //            管道已经满了，write会阻塞
    //            管道没有满，write将数据写入，并返回实际写入的字节数。
    // 判断文件是否存在
    int ret = access("test", F_OK);
    if (ret == -1) {
        // create fifo
        // mode: 文件权限，和open的mode相同 = (mode & ~umask)
        // 八进制，0开头表示8进制，后面644是权限
        ret = mkfifo("test", 0664);
        if (ret == -1) {
            perror("mkfifo failed");
            exit(0);
        }
    }
    // only write open fifo
    // 读端没有收到消息，则会阻塞在open这里
    int fd = open("test", O_WRONLY);
    assert(fd != -1);

    // 可以用dup2把标准输出重定向到fd中，而不是标准输入进行重定向
    dup2(fd, STDOUT_FILENO);
    string str;
    //用getline从标准输入中获取数据，回车停止读入，只要有cin流就会返回the number of characters read,
    while (getline(cin, str)) {
        cout << str << endl;//通过标准输出将读入的数据打印出来
    }

    //    for (int i = 0; i < 100; i++) {
    //        char buf[1024];
    //        sprintf(buf, "hello: %d\n", i);
    //        printf("write data:%s\n", buf);
    //        write(fd, buf, strlen(buf));
    //        sleep(1);
    //    }

    close(fd);
    return 0;
}