//
// Created by baitianyu on 5/12/24.
//
#include "arpa/inet.h"
#include "assert.h"
#include "errno.h"
#include "fcntl.h"
#include "iostream"
#include "string.h"
#include "sys/select.h"
#include "sys/sendfile.h"
#include "sys/socket.h"
#include "sys/uio.h"
#include "unistd.h"
#include <sys/stat.h>
using namespace std;

#define BUF_SIZE 1024

int main() {

    int port = 22225;
    const char *ip = "127.0.0.1";
    int backlog = 5;

    // create addr
    sockaddr_in listen_addr;
    listen_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &listen_addr.sin_addr);
    listen_addr.sin_family = AF_INET;

    // create socket
    int listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listen_fd != -1);

    // bind and listen
    int ret = bind(listen_fd, (sockaddr *) &listen_addr, sizeof(listen_addr));
    assert(ret != -1);
    ret = listen(listen_fd, backlog);
    assert(ret != -1);

    // accept
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int connfd = accept(listen_fd, (sockaddr *) &client_addr, &client_addr_len);
    if (connfd < 0) {
        cout << "connection failed" << endl;
        close(listen_fd);
    } else {
        char buff[BUF_SIZE];
        fd_set read_fds;
        fd_set exception_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&exception_fds);// fdset位设置为0
        while (1) {
            memset(buff, '\0', sizeof(buff));
            // 每次调用select之前都要重新设置fds，因为事件发生后，fdset会被内核所修改
            FD_SET(connfd, &read_fds);
            FD_SET(connfd, &exception_fds);
            // select调用返回时，内核将修改它们来通知应用程序哪些文件描述符已经就绪。
            // nfds 的参数是connfd+1，通常被设置为select监听的所有文件描述符中的最大值加1，因为文件描述符是从0 开始计数的。
            ret = select(connfd+1, &read_fds, NULL, &exception_fds, NULL);
            if (ret < 0) {
                cout << "selection failure" << endl;
                break;
            }
            // fd是否被设置
            if (FD_ISSET(connfd, &read_fds)) {
                // 普通数据用recv
                ret = recv(connfd, buff, sizeof(buff) - 1, 0);
                if (ret <= 0) { //字节数为0也要返回
                    cout<<"get normal data ret: "<<ret<<endl;
                    break;
                }
                cout << "get normal data: " << ret << " " << buff << endl;
            } else if (FD_ISSET(connfd, &exception_fds)) {
                // 异常数据用msgobj
                ret = recv(connfd, buff, sizeof(buff) - 1, MSG_OOB);
                if (ret <= 0) {
                    cout<<"get oob data ret: "<<ret<<endl;
                    break;
                }
                cout << "get oob data: " << ret << " " << buff << endl;
            }
        }
        close(connfd);
        close(listen_fd);
        return 0;
    }
}