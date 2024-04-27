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

#define BUFF_SIZE 1024

int main() {
    int port = 22224;
    const char *ip = "127.0.0.1";
    // backlog通常被描述为用于限制传入的连接数
    int backlog = 5;

    // 创建地址
    // server中该address负责监听地址，然后accept会返回N个新的客户端的地址，说明有一个专门监听，剩下的都在处理数据传输啥的
    sockaddr_in base_address;
    memset(&base_address, 0, sizeof(base_address));
    base_address.sin_port = htons(port);
    base_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &base_address.sin_addr);

    // 创建socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);
    int ret = bind(sockfd, (sockaddr *) &base_address, sizeof(base_address));
    assert(ret != -1);

    // 监听socket
    ret = listen(sockfd,backlog);
    assert(ret != -1);

    // 创建客户端的地址
    sockaddr_in client_address;
    // sockfd参数是监听socket
    // addr参数用来获取被接受连接的远端socket地址，该socket地址的长度由addrlen参数指出。
    socklen_t client_addr_len = sizeof (client_address);
    int connfd = accept(sockfd, (sockaddr *) &client_address, &client_addr_len);
    if (connfd<0){
        cout<<"connection failed"<<endl;
    }else{
        char buffer[BUFF_SIZE]; // 有'\0'作为停止符，所以字符串最大长度实际上是BUFF_SIZE-1
        memset(buffer,'\0',BUFF_SIZE);
        int ret = recv(connfd,buffer,BUFF_SIZE-1,0);
        cout<<"get: "<<buffer<<", len: "<<ret<<endl;

        memset(buffer,'\0',BUFF_SIZE);
        ret = recv(connfd,buffer,BUFF_SIZE-1,MSG_OOB); // 紧急数据
        cout<<"get: "<<buffer<<", len: "<<ret<<endl;

        memset(buffer,'\0',BUFF_SIZE);
        ret = recv(connfd,buffer,BUFF_SIZE-1,0);
        cout<<"get: "<<buffer<<", len: "<<ret<<endl;

        close(connfd);
    }
    close(sockfd);
    return 0;

}