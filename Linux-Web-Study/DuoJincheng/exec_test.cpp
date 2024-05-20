//
// Created by baitianyu on 5/19/24.
//
#include "iostream"
#include "sys/types.h"
#include "unistd.h"
using namespace std;

int main() {
    pid_t pid = fork();
    if (pid > 0) {
        cout << "parent" << endl;
    } else if (pid == 0) {
        // The list of arguments must be terminated by a null pointer
        // 第一个参数没有什么作用，一般写的是执行的程序的名称
        // The first argument, by convention,
        // should point to the filename associated with the file being executed.
        //        execl("/bin/ps","ps","aux", nullptr); // 调用失败返回-1，返回errno

        //        char *args[] = {"ps","aux", nullptr};
        //        execv("/bin/ps",args); // 调用失败返回-1，返回errno

        // 这与在bin目录下执行 ls -al /etc/passwd 所得到的结果是一样的。
        // envp 同样也是参数序列，一般来说他是一种键值对的形式 key=value. 作为我们是新程序的环境。
        char *argv[]={"ls", "-al", "/etc/passwd", NULL};
        char *envp[]={"PATH=/bin", NULL};
        execve("/bin/ls", argv, envp);


        perror("execl failed!");// 出错了输出
        // 已经被替换了，所以不会输出到这
        cout << "child" << endl;
    } else {
        cout << "error" << endl;
    }
}