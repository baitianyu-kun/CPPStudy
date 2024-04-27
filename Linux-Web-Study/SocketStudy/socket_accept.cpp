#include "errno.h"
#include <arpa/inet.h>
#include <assert.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// 对于类 Unix 系统，unistd.h 中所定义的接口通常都是大量针对系统调用的封装（英语：wrapper functions），
// 如 fork、pipe 以及各种 I/O 原语（read、write、close 等等）。
using namespace std;

int main() {
    int port = 22223;
    const char *ip = "192.168.182.128";
    int backlog = 5;

    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    int ret = bind(sockfd, (sockaddr *) &address, sizeof(address));
    assert(ret != -1);

    ret = listen(sockfd, backlog);
    assert(ret != -1);

    sleep(10);

    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    // connfd是返回的新套接字的描述符
    int connfd = accept(sockfd, (sockaddr *) &client_address, &client_address_len);


    // accept 只是从监听队列中取出连接，
    // 而不论连接处于何种状态(如上面的 ESTABLISHED 状态和CLOSE_WAIT 状态)，更不关心任何网络状况的变化。
    if (connfd < 0) {
        // errno 是一个全局变量，用于在发生错误时存储错误码。
        // 它定义在 <errno.h> 头文件中，并被许多操作系统和库函数使用。
        cout << "errno: " << errno << endl;
    } else {
        char remote[INET_ADDRSTRLEN];// ipv4的地址长度
        cout << "ip: " << inet_ntop(AF_INET, &client_address.sin_addr, remote, INET_ADDRSTRLEN) << endl;
        cout << "port: " << ntohs(client_address.sin_port);
        close(connfd);// 关闭接收
    }
    close(sockfd);
}