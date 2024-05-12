//
// Created by baitianyu on 5/12/24.
//
#include "arpa/inet.h"
#include "assert.h"
#include "errno.h"
#include "iostream"
#include "string.h"
#include "sys/socket.h"
#include <unistd.h>
using namespace std;

int main() {
    // client,需要使用connect主动和服务器做连接
    int port = 22225;
    const char *ip = "127.0.0.1";
    int backlog = 5;

    // 创建地址
    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);

    // 创建socket(匿名的，不用绑定)
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);
    // 在服务器程序中，我们通常要命名socket，因为只有命名后客户端才能知道该如何连接它。
    // 客户端则通常不需要命名socket，而是采用匿名方式，即使用操作系统自动分配的socket地址。
    // int ret = bind(sockfd, (sockaddr *) &server_address, sizeof(server_address));
    // assert(ret != -1);

    // client主动连接server，而server只需要被动listen
    if (connect(sockfd, (sockaddr *) &server_address, sizeof(server_address)) < 0) {
        cout << "connection failed" << endl;
    } else {
        // out of band 带外数据，即紧急事件，因为只有一个紧急指针，这也意味着它只能标识一个字节的数据。这个指针指向了紧急数据最后一个字节的下一个字节。
        // 所以接收时候，只能收到一个字节的紧急数据，如果不能及时取出的话就会被覆盖，并且会截断普通数据的传输
        // 16位，占用两个字节，但是其总是指向紧急数据的下一位，所以其只能标识1个字节的数据
        // 紧急指针被设置为指向最后一个带外数据的下一字节
        char *oob_data = "a";
        char *normal_data = "normalsdata123";
        send(sockfd, normal_data, strlen(normal_data), 0);
        //send(sockfd, oob_data, strlen(oob_data), MSG_OOB);
    }
    close(sockfd);
    return 0;
}