//
// Created by baitianyu on 4/27/24.
//
#include "arpa/inet.h"
#include "assert.h"
#include "iostream"
#include "string.h"
#include "sys/socket.h"
#include "unistd.h"
using namespace std;

#define BUF_SIZE 1024

int main() {

    int port = 22225;
    const char *ip = "127.0.0.1";
    int backlog = 5;

    // 创建地址
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    // 创建socket
    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    assert(sockfd != -1);
    int ret = bind(sockfd, (sockaddr *) &address, sizeof(address));
    assert(ret != -1);

    // listen
    ret = listen(sockfd, backlog);
    assert(ret != -1);

    sockaddr_in client_address;


}