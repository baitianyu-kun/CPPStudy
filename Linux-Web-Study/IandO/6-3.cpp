//
// Created by baitianyu on 5/6/24.
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

#define BUFF_SIZE 1024

static const char *status_line[2] = {"200 OK", "500 INTERNAL SERVER ERROR"};

int main() {

    int port = 22221;
    const char *ip = "127.0.0.1";
    const char *file_name = "/home/baitianyu/Linux-Web-Study/test.txt";

    // create
    sockaddr_in listenaddr;
    memset(&listenaddr, 0, sizeof(listenaddr));
    listenaddr.sin_family = AF_INET;
    listenaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &listenaddr.sin_addr);

    // create bind and listen socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);
    int ret = bind(sockfd, (sockaddr *) &listenaddr, sizeof(listenaddr));
    assert(ret != -1);
    ret = listen(sockfd, 5);
    assert(ret != -1);

    // get file
    int filefd = open(file_name, O_RDONLY);
    struct stat stat_buf;
    fstat(filefd, &stat_buf);

    // get client
    sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    int connfd = accept(sockfd, (sockaddr *) &clientaddr, &clientaddr_len);
    if (connfd < 0) {
        cout << "connection failed" << endl;
    } else {
        sendfile(connfd, filefd, NULL, stat_buf.st_size);
        close(connfd);
    }
    close(sockfd);
    return 0;
}