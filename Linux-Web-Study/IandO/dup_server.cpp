//
// Created by baitianyu on 5/6/24.
//
#include "arpa/inet.h"
#include "assert.h"
#include "iostream"
#include "string.h"
#include "sys/socket.h"
#include "unistd.h"
using namespace std;

int main() {

    int port = 22227;
    const char *ip = "127.0.0.1";

    // create address
    sockaddr_in listenaddr;
    memset(&listenaddr, 0, sizeof(listenaddr));
    listenaddr.sin_family = AF_INET;
    listenaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &listenaddr.sin_addr);

    // create bind and listen
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);
    int ret = bind(sockfd, (sockaddr *) &listenaddr, sizeof(listenaddr));
    assert(ret != -1);
    ret = listen(sockfd, 5);
    assert(ret != -1);

    // accept connection
    sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    int connfd = accept(sockfd, (sockaddr *) &clientaddr, &clientlen);
    if (connfd < 0) {
        cout << "connection failed" << endl;
    } else {
        //close(STDOUT_FILENO); // 关闭标准输入
        //dup(connfd); // 然后dup，返回最小文件描述符，即1，即为之前的STDOUT_FILENO，STDOUT_FILENO默认就为1

        //or, 把后面的指向前面的进行输出
        dup2(connfd,STDOUT_FILENO);
        cout << "hello world this is my dup from server" << endl;
        close(connfd);
    }
    close(sockfd);
}