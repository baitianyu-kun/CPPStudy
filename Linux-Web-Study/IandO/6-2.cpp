//
// Created by baitianyu on 5/6/24.
//
#include "arpa/inet.h"
#include "assert.h"
#include "errno.h"
#include "fcntl.h"
#include "iostream"
#include "string.h"
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

    // get client
    sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    int connfd = accept(sockfd, (sockaddr *) &clientaddr, &clientaddr_len);
    if (connfd < 0) {
        cout << "connection failed" << endl;
    } else {
        char header_buf[BUFF_SIZE];
        memset(header_buf, '\0', BUFF_SIZE);// 存http应答行，头部字段和一个空行的缓冲区

        char *file_buf;// 存放目标文件内容的应用程序缓存
        // 有结构体和函数同名，所以需要这里加上struct
        struct stat file_stat;// 获取目标文件的属性，比如是否为目录和文件大小
        bool valid = true;    // 记录文件是否为有效文件
        int len = 0;          // header buf 目前已经使用了多少字节的空间
        if (stat(file_name, &file_stat) < 0) {
            valid = false;// 目标文件不存在
        } else {
            if (S_ISDIR(file_stat.st_mode)) {
                valid = false;// 目标文件为目录
            } else if (file_stat.st_mode & S_IROTH) {
                // 当前用户有读取权限，S_IROTH为其他读
                int fd = open(file_name, O_RDONLY);
                // 动态分配缓存区
                file_buf = new char[file_stat.st_size + 1];
                memset(file_buf, '\0', file_stat.st_size + 1);
                if (read(fd, file_buf, file_stat.st_size) < 0) {
                    // 读取文件并判断是否失败
                    valid = false;
                }
            } else {
                valid = false;
            }
        }
        if (valid) {
            ret = snprintf(header_buf, BUFF_SIZE - 1, "%s %s\r\n",
                           "HTTP/1.1", status_line[0]);
            len += ret;
            ret = snprintf(header_buf + len, BUFF_SIZE - 1 - len,
                           "Content-Length: %d\r\n", file_stat.st_size);
            len += ret;
            ret = snprintf(header_buf + len, BUFF_SIZE - 1 - len, "%s", "\r\n");

            struct iovec iv[2];
            iv[0].iov_base = header_buf;
            iv[0].iov_len = strlen(header_buf);
            iv[1].iov_base = file_buf;
            iv[1].iov_len = file_stat.st_size;
            // writev集中写
            ret = writev(connfd, iv, 2);
        } else {
            // 目标文件无效通知客户端发生内部错误
            ret = snprintf(header_buf, BUFF_SIZE - 1, "%s %s\r\n",
                           "HTTP/1.1", status_line[1]);
            len + ret;
            ret = snprintf(header_buf + len, BUFF_SIZE - 1 - len, "%s", "\r\n");
            send(connfd, header_buf, strlen(header_buf), 0);
        }
        close(connfd);
        delete[] file_buf;
    }
    close(sockfd);
    return 0;
}