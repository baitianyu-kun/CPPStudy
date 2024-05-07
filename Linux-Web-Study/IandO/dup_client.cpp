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

#define BUFF_SIZE 1024

int main() {

    int port = 22227;
    const char *ip = "127.0.0.1";

    // create addr
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    // create socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);

    // connection
    if (connect(sockfd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cout << "connection server failed" << endl;
    } else {
        char buffer[BUFF_SIZE];
        memset(buffer, '\0', BUFF_SIZE);
        int ret = recv(sockfd, buffer, BUFF_SIZE - 1, 0);
        cout << "get: " << buffer << ", len: " << ret << endl;
    }
    close(sockfd);
}