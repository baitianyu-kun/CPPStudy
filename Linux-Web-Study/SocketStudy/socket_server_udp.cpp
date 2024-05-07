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

    int port = 22226;
    const char *ip = "127.0.0.1";
    int backlog = 5;

    // 创建地址
    sockaddr_in base_address;
    memset(&base_address, 0, sizeof(base_address));
    base_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &base_address.sin_addr);
    base_address.sin_family = AF_INET;

    // 创建socket
    int sockfd = socket(PF_INET,SOCK_DGRAM,0);
    assert(sockfd!=-1);
    int ret = bind(sockfd,(sockaddr *)&base_address,sizeof (base_address));
    assert(ret!=-1);

    // 接收数据
    // 创建客户端的地址
    sockaddr_in client_address;
    // sockfd参数是监听socket
    // addr参数用来获取被接受连接的远端socket地址，该socket地址的长度由addrlen参数指出。
    socklen_t client_addr_len = sizeof (client_address);

    char buffer[BUF_SIZE]; // 有'\0'作为停止符，所以字符串最大长度实际上是BUFF_SIZE-1
    // tcp才需要connect和accept和listen，udp不需要，revcfrom是拥塞函数，没有数据就一直拥塞
    while (1){
        memset(buffer,0,BUF_SIZE);
        // recvfrom是拥塞函数，没有数据就一直拥塞
        ssize_t ret2 = recvfrom(sockfd,buffer,BUF_SIZE-1,0,(sockaddr *)&client_address,&client_addr_len);
        cout<<"data: "<<buffer<<" len: "<<ret2<<endl;
        memset(buffer,0,BUF_SIZE);
    }
}