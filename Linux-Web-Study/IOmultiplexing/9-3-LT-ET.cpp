//
// Created by baitianyu on 5/12/24.
//
#include "arpa/inet.h"
#include "assert.h"
#include "errno.h"
#include "fcntl.h"
#include "iostream"
#include "pthread.h"
#include "string.h"
#include "sys/epoll.h"
#include "sys/select.h"
#include "sys/sendfile.h"
#include "sys/socket.h"
#include "sys/uio.h"
#include "unistd.h"
#include <sys/stat.h>
using namespace std;

#define BUF_SIZE 10
#define MAX_EVENT 1024

// 描述符设置为非阻塞
int set_non_blocking(int fd) {
    // 根据操作类型的不同，该函数可能还需要第三个可选参数arg，例如设置新的描述符的状态。
    int old_option = fcntl(fd, F_GETFL);// 获取和设置文件描述符的状态标志
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

// 将文件描述符fd上的EPOLLIN事件（毕竟可读可写都是某个具体的fd，而epollfd代表注册一个事件表）注册到epollfd指示的epoll内核事件表中，enable et代表是否启用et模式
// 当epoll_wait检测到其上有事件发生并将此通知应用程序后，应用程序必须立即处理该事件，et，而lt不用立即处理
// 因为过一会会重新进行通知
void addfd(int epollfd, int fd, bool enable_ET) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (enable_ET) {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    // 每个使用ET模式的文件描迷符都应该是非阻塞的。如果文件描述符是阻塞的，那么 读或写探作将会因为没有后续的事件而一直处于阻塞状态(饥渴状态)。
    set_non_blocking(fd);
}

void lt(epoll_event *events, int number, int epollfd, int listenfd) {
    char buf[BUF_SIZE];
    for (int i = 0; i < number; i++) {
        int sockfd = events[i].data.fd;
        if (sockfd == listenfd) {
            struct sockaddr_in client_address;
            socklen_t client_address_len = sizeof(client_address);
            int connfd = accept(listenfd, (sockaddr *) &client_address, &client_address_len);
            addfd(epollfd, connfd, false);// 对connfd禁用et
        } else if (events[i].events & EPOLLIN) {
            cout << "event trigger once" << endl;
            memset(buf, '\0', BUF_SIZE);
            // 通常是为了预留一个字节来放置字符串的结尾标志（null 终止符），以便在之后对接收的数据进行字符串操作。
            int ret = recv(sockfd, buf, BUF_SIZE - 1, 0);
            if (ret <= 0) {
                close(sockfd);
                continue;
            }
            cout << "get data: " << ret << " " << buf << endl;
        } else {
            cout << "unknown error" << endl;
        }
    }
}

void et(epoll_event *events, int number, int epollfd, int listenfd) {
    char buf[BUF_SIZE];
    for (int i = 0; i < number; i++) {
        int sockfd = events[i].data.fd;
        if (sockfd == listenfd) {
            struct sockaddr_in client_address;
            socklen_t client_address_len = sizeof(client_address);
            int connfd = accept(listenfd, (sockaddr *) &client_address, &client_address_len);
            addfd(epollfd, connfd, true);// 对connfd开启et
        } else if (events[i].events & EPOLLIN) {
            cout << "event trigger once" << endl;
            // 由于et模式只会被触发一次，所以需要循环将socket上的数据读完
            while (1) {
                memset(buf, '\0', BUF_SIZE);
                int ret = recv(sockfd, buf, BUF_SIZE - 1, 0);
                if (ret < 0) {
                    // 对于非阻塞IO，下面的条件成立表示数据已经全部读取。此后，epoll就能再次触发sockfd上的EPOLLIN事件，进行下一次事件
                    if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                        cout << "read ok" << endl;
                        break;
                    }
                    close(sockfd);
                    break;
                } else if (ret == 0) {
                    close(sockfd);
                } else {
                    cout << "get data: " << ret << " " << buf << endl;
                }
            }
        } else {
            cout << "unknown error" << endl;
        }
    }
}

int main() {
    // baitianyubaitianyubaitianyubaitianyu
    // et 1次，lt 5次

    int port = 22226;
    const char *ip = "127.0.0.1";
    int backlog = 5;

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    int ret = bind(listenfd, (sockaddr *) &address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, backlog);
    assert(ret != -1);

    //create epoll events
    // 创建epoll，创建epoll事件表，将fd添加到epoll事件表中，然后使用epollwait，fd就绪了的话就拷贝到events中
    // 就绪后访问拷贝过的events事件表，如果当前fd为listenfd的话，说明有客户端来连接了，
    // 然后accept连接，并把新的connfd放到events中，设置为是否启用et和lt
    // 然后跳转到while上面，继续epoll wait，当监听到connfd中的可读就绪的时候，那我们就开始使用et或者lt来进行读
    epoll_event events[MAX_EVENT];
    int epollfd = epoll_create(5); // 创建事件表
    assert(epollfd != -1);
    addfd(epollfd, listenfd, true);

    while (1) {
        int ret = epoll_wait(epollfd, events, MAX_EVENT, -1);
        if (ret < 0) {
            cout << "epoll failed" << endl;
            break;
        }
        et(events, ret, epollfd, listenfd);
    }
    close(listenfd);
    return 0;
}