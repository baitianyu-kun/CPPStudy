//
// Created by baitianyu on 5/20/24.
//
#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "unistd.h"
using namespace std;

int main() {
    // 创建10个子进程
    pid_t pid;
    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0)
            break;
    }
    // 使用waitpid进行监听
    if (pid > 0) {
        // 父进程
        while (1) {
            cout << "parent: " << getpid() << endl;
            sleep(1);
            int status;
            // 非阻塞
            // 如果pid指定的目标子进程还没有结束或意外终止，则waitpid立即返回0，如果目标子进程确实正常退出了，则waitpid返回该子进程的PID。
            // -1的话等待任意一个子进程结束，和wait一样了，否则为要监听的进程
            // WNOHANG常量的作用是告诉waitpid函数在没有子进程退出时立即返回，而不会阻塞等待。
            int ret = waitpid(-1, &status, WNOHANG);
            if (ret == -1) {
                // 所有进程都退出后，返回-1
                break;
            } else if (ret == 0) {
                // 有没有退出的continue
                continue;
            }else if (ret>0){
                // 退出
                if (WIFEXITED(status)) {
                    // 返回非0值，正常退出
                    cout << "exit code: " << WEXITSTATUS(status) << endl;
                }
                cout << "child end, pid: " << ret << endl;
            }
        }
    } else if (pid == 0) {
        cout << "this is child" << endl;
        sleep(1);
        exit(0);// 退出进程
    }
    return 0;
}