//
// Created by baitianyu on 5/20/24.
//
#include "assert.h"
#include "cstring"
#include "fcntl.h"
#include "iostream"
#include "unistd.h"
using namespace std;

int main() {
    // 匿名管道，由于没有名字，只能用于亲缘关系的进程间通信。
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

        // set read non-block
        int old_flags = fcntl(pipefd[0], F_GETFL);
        int new_flags = old_flags | FNONBLOCK;
        fcntl(pipefd[0], F_SETFL, new_flags);

        while (1) {
            // nonblock的时候read会立即返回-1，不会像之前一样阻塞
            // block的时候，无数据，read阻塞
            // block的时候，管道满了，write阻塞
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("len : %d\n", len);
            printf("parent recv : %s, pid : %d\n", buf, getpid());
            memset(buf, '\0', 1024); // 清空数组
            sleep(1);
        }
    } else if (pid == 0) {
        // child
        printf("child process:%d\n", getpid());
        close(pipefd[0]);
        while (1) {
            char *str = "hello this is child process";
            write(pipefd[1], str, strlen(str));
            sleep(5);
        }
    }
}