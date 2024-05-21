//
// Created by baitianyu on 5/20/24.
//
#include "assert.h"
#include "iostream"
#include "string.h"
#include "unistd.h"
using namespace std;

int main() {

    // 管道默认是阻塞的：如果管道中没有数据，read阻塞，如果管道满了，write阻塞
    // 注意：匿名管道只能用于具有关系的进程之间的通信（父子进程，兄弟进程）
    // 管道读写端都有引用计数
    int pipefd[2];
    int ret = pipe(pipefd);
    assert(ret != -1);

    // 创建子进程
    pid_t pid = fork();
    if (pid > 0) {
        // father
        printf("parent process:%d\n", getpid());
        // close write
        close(pipefd[1]);
        // read
        char buf[1024];
        memset(buf, '\0', 1024);
        while (1) {
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("parent recv:%s, pid:%d\n", buf, getpid());
        }
    } else if (pid == 0) {
        // child
        printf("child process:%d\n", getpid());
        close(pipefd[0]);
        while (1){
            char *str = "hello this is child process";
            write(pipefd[1], str, strlen(str));
            sleep(1);
        }
    }
}
