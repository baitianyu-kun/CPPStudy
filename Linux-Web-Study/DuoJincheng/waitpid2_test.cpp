//
// Created by baitianyu on 5/20/24.
//
#include "iostream"
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "unistd.h"
using namespace std;

void handle_child(int sigchild) {
    int status;
    pid_t pid;
    // 每个子进程死亡以后，发送SIGCHLD信号给父进程，然后父进程中捕获到SIGCHLD信号，
    // 内核调用一次catchFunc来回收该子进程，循环往复… 其实这是有问题的，因为一个子进程死亡后，
    // 在执行catchFunc的过程中又有多个子进程死亡而发送SIGCHLD信号，我们之前说过，某个信号捕捉函数执行期间，
    // 该信号自动被屏蔽，所以紧接着死亡的这些子进程就无法被回收。
    // 而解决这一问题的方法是：在catchFunc捕捉函数中int ret = waitpid(-1, NULL, WNOHANG);设置为非阻塞，
    // 并加上死循环，让其一次调用多次回收，直到当前没有死亡的子进程了，就break出来继续执行父进程的代码。
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("parent pid:%d, handled sig:%d, child pid:%d\n", getpid(), sigchild, pid);
    }
}

int main() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);

    // 而当你在父进程中使用sigprocmask(SIG_BLOCK, &set, NULL)阻塞SIGCHLD信号时,并不会阻止子进程退出。
    // 子进程仍然会正常退出。但是,由于 SIGCHLD 信号被阻塞,内核不会立即向父进程发送这个信号。
    // ===================
    // 所以也就是说，子进程可能退出了一会以后，父进程才收到信号，但是这也不影响父进程去处理子进程的后续信息。
    // 如果不阻塞的话，那么子进程立即退出，内核发送SIGCHLD，但是父进程可能还没注册好，
    // 所以SIGCHLD就被丢弃了，然后父进程也收回不了资源，就变成了僵尸进程
    // ===================
    // 父进程通过阻塞SIGCHLD信号，确保在注册SIGCHLD信号处理函数之前不会接收到该信号。
    // 这样做的目的是为了避免在注册处理函数之前就收到子进程退出的信号，从而导致处理函数无法正确执行。
    // 应当提前设置好阻塞信号集，阻塞SIGCHLD，因为有可能子进程很快结束，父进程还没有注册完信号捕捉。
    // 等到父进程执行int res = sigaction(SIGCHLD, &act, NULL);完成SIGCHLD信号捕捉的注册后，
    // 再使用sigprocmask(SIG_UNBLOCK, &set, NULL);解除阻塞。
    // -set参数指定新的信号掩码，oset参数则输出原来的信号掩码（如果不为NULL的话)。
    // 如果_set参数不为NULL，则how参数指定设置进程信号掩码的方式
    sigprocmask(SIG_BLOCK, &set, NULL);

    // 创建10个子进程
    pid_t pid;
    int i;
    for (i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0)
            break;
    }
    if (pid > 0) {
        printf("parent created pid:%d\n", getpid());
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = handle_child;
        sigemptyset(&act.sa_mask);
        // 阻塞，sigaction函数有阻塞的功能，比如SIGINT信号来了，进入信号处理函数，
        // 默认情况下，在信号处理函数未完成之前，如果又来了一个SIGINT信号，其将被阻塞，只有信号处理函数处理完毕，
        // 才会对后来的SIGINT再进行处理，同时后续无论来多少个SIGINT，
        // 仅处理一个SIGINT，sigaction会对后续SIGINT进行排队合并处理。

        // act参数指定新的信号处理方式，oact参数则输出信号先前的处理方式(如果不为NULL的话)。
        int res = sigaction(SIGCHLD, &act, nullptr);
        if (res == -1) {
            perror("sig error");
            exit(-1);
        }
        // 注册完信号捕捉以后，解除阻塞
        sigprocmask(SIG_UNBLOCK, &set, nullptr);

        // 在阻塞期间，无法收到sigchild信号，然后，在子进程执行sleep3的时候，
        // 可能父进程已经执行完了，所以没办法回收调用，所以需要等待一下子进程的操作
        while (true) {
            sleep(1);
        }

    } else if (pid == 0) {
        printf("child created pid:%d\n", getpid());
        exit(0);// 退出进程
    }
}
