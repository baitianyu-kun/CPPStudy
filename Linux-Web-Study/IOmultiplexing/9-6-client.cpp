//
// Created by baitianyu on 5/13/24.
//
#include "arpa/inet.h"
#include "assert.h"
#include "errno.h"
#include "fcntl.h"
#include "iostream"
#include "poll.h"
#include "pthread.h"
#include "string.h"
#include "sys/epoll.h"
#include "sys/select.h"
#include "sys/sendfile.h"
#include "sys/socket.h"
#include "sys/uio.h"
#include "unistd.h"
#include <sys/stat.h>
using namespace std;

#define BUF_SIZE 1024

int main() {
    int port = 22225;
    const char *ip = "127.0.0.1";
    int backlog = 5;

    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);

    if (connect(sockfd, (sockaddr *) &server_address, sizeof(server_address)) < 0) {
        cout << "connection failed" << endl;
        close(sockfd);
        return -1;
    }

    // 将标准输入读入，并发送到服务器
    // 往标准输出中打印服务器发送的内容
    pollfd fds[2];
    // 注册文件描述符0，即标准输入STDIN_FILENO
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    //fds[0].revents = STDIN_FILENO; // 由内核修改发生了的事件
    // 注册sockfd上的可读事件
    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;//对方关闭了写或者关闭了tcp操作
    //fds[1].revents = STDIN_FILENO; // 由内核修改发生了的事件

    char buff[BUF_SIZE];

    // 创建pipe，用于splice零拷贝函数
    int pipefd[2];
    int ret = pipe(pipefd);
    assert(ret != -1);

    while (1) {
        ret = poll(fds, 2, -1);
        if (ret < 0) {
            cout << "poll failure" << endl;
            break;
        }
        if (fds[1].revents & POLLRDHUP) {
            cout << "connection closed" << endl;
            break;
        } else if (fds[1].revents & POLLIN) {
            // 打印事件
            memset(buff, '\0', BUF_SIZE);
            recv(fds[1].fd, buff, BUF_SIZE - 1, 0);
            cout << buff << endl;
        }
        if (fds[0].revents & POLLIN) {
            // 使用splice将用户数据写道sockfd上
            // 发送事件
            ret = splice(STDIN_FILENO, NULL, pipefd[1],
                         NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
            ret = splice(pipefd[0], NULL, sockfd,
                         NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        }
    }
    close(sockfd);
    return 0;
}