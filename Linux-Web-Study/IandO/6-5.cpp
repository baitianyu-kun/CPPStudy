//
// Created by baitianyu on 5/7/24.
//
#include "arpa/inet.h"
#include "assert.h"
#include "errno.h"
#include "fcntl.h"
#include "iostream"
#include "string.h"
#include "sys/sendfile.h"
#include "sys/socket.h"
#include "sys/uio.h"
#include "unistd.h"
#include <sys/stat.h>
using namespace std;

int main() {

    // 实现了Linux下tee程序(同时输出数据到终端和文件的程序)
    // tee在两个管道文件描述符之间复制数据
    // tee签名fd_in待输入数据的文件描述符，必须是管道文件，所以需要用两个管道
    char *file_name = "/home/baitianyu/Linux-Web-Study/test.txt";
    int filefd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    assert(filefd > 0);

    int pipefd_stdout[2];
    int ret = pipe(pipefd_stdout);
    assert(ret != -1);

    int pipefd_file[2];
    ret = pipe(pipefd_file);
    assert(ret != -1);

    // pipe1是输入，pipe0是读取，把标准输入放到管道中
    ret = splice(STDIN_FILENO, NULL, pipefd_stdout[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);

    // pipefd stdout的内容复制到pipefd_file中
    ret = tee(pipefd_stdout[0], pipefd_file[1], 32768, SPLICE_F_NONBLOCK);
    assert(ret != -1);

    // pipefd file的内容移动到filefd中，实现向文件写入
    ret = splice(pipefd_file[0], NULL, filefd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);

    // pipefd stdout定向到标准输出，其内容和写入的文件完全一致
    ret = splice(pipefd_stdout[0], NULL, STDOUT_FILENO, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);

    close(filefd);
    close(pipefd_stdout[0]);
    close(pipefd_stdout[1]);
    close(pipefd_file[0]);
    close(pipefd_file[1]);
    return 0;
}