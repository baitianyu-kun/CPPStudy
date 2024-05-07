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

int main(){

    int port = 22226;
    const char *ip = "127.0.0.1";

    // 创建地址
    sockaddr_in server_address;
    memset(&server_address,0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port= htons(port);
    inet_pton(AF_INET,ip,&server_address.sin_addr);

    // 创建socket
    int sockfd = socket(PF_INET,SOCK_DGRAM,0);
    assert(sockfd!=-1);

    // 发送数据
    // sizeof(data)将返回指针变量data的大小，而不是它所指向的字符串的大小。在大多数系统上，指针的大小通常是4或8个字节（取决于系统的位数）。因此，输出的结果为8。
    char * normal_data = "hellosudp1231231231231231";
    sendto(sockfd,normal_data, strlen(normal_data),0,(sockaddr *)&server_address, sizeof(server_address));

}