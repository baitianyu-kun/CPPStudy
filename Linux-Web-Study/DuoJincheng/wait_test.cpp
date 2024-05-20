//
// Created by baitianyu on 5/20/24.
//
#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "wait.h"
using namespace std;

int main() {

    // 创建10个子进程
    pid_t pid;
    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0)
            break;
    }

    // 使用wait进行监听
    if (pid > 0) {
        // 父进程
        while (1) {
            cout << "parent: " << getpid() << endl;
            int status;
            int ret = wait(&status);
            if (ret == -1) {
                // 所有进程都退出后，返回-1
                break;
            }
            if (WIFEXITED(status)) {
                // 返回非0值，正常退出
                cout << "exit code: " << WEXITSTATUS(status) << endl;
            }
            // exit signal: 9
            // kill -9 47992
            if (WIFSIGNALED(status)) {
                cout << "exit signal: " << WTERMSIG(status) << endl;
            }
            cout << "child end, pid: " << ret << endl;
            sleep(1);
        }
    } else if (pid == 0) {
        cout<<"this is child"<<endl;
        sleep(1);
        exit(0); // 退出进程
//        while (1) {
//            cout<<"this is child"<<endl;
//            sleep(1);
//        }
//        exit(0);
    }
    return 0;
}