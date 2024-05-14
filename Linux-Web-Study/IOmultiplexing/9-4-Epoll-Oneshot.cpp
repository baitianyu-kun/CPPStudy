//
// Created by baitianyu on 5/12/24.
//
#include "arpa/inet.h"
#include "errno.h"
#include "fcntl.h"
#include "iostream"
#include "pthread.h"
#include "string.h"
#include "sys/epoll.h"
#include "sys/sendfile.h"
#include "sys/socket.h"
#include "unistd.h"
using namespace std;

// 期望的是一个socket连接在任一时刻都只被一个线程处理。这一点可以使用epoll的EPOLLONESHOT事件实现。

#define BUF_SIZE 10
#define MAX_EVENT 1024

struct fds {
    int epollfd;
    int sockfd;
};

int set_non_blocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd, bool enable_oneshot) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if (enable_oneshot) {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    set_non_blocking(fd);
}

// 如果对描述符socket注册了EPOLLONESHOT事件，那么操作系统最多触发其上注册的一个可读、可写或者异常事件，且只触发一次。
// 想要下次再触发则必须使用epoll_ctl重置该描述符上注册的事件，包括EPOLLONESHOT事件。
void reset_oneshot(int epollfd, int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

// fds结构体，工作线程需要做的事情
// 线程函数的参数通常是一个指向任意类型数据的指针，因为线程函数需要能够接受不同类型的参数。
// 返回值和参数都必须为void *
void *worker(void *arg) {
    pthread_t id = pthread_self(); // 获取Pthreads线程id

    int sockfd = ((fds *) arg)->sockfd;
    int epollfd = ((fds *) arg)->epollfd;
    cout << "new thread to receive data on fd: " << sockfd<<" tid: "<<id << endl;
    char buf[BUF_SIZE];
    while (1) {
        // 循环读数据应该放到这里
        memset(buf, '\0', BUF_SIZE);
        // 循环读数据
        int ret = recv(sockfd, buf, BUF_SIZE - 1, 0);
        if (ret == 0) {
            close(sockfd);
            break;
        } else if (ret < 0) {
            // 对于非阻塞IO，下面的条件成立表示数据已经全部读取。
            if (errno == EAGAIN) {
                // 全部读取后，来新的数据需要再次触发，所以需要再次注册oneshot事件
                reset_oneshot(epollfd, sockfd);
                cout << "read ok" << endl;
                break;
            }
        } else {
            cout << "get data: " << buf << endl;
            sleep(1);
        }
    }
    cout << "end thread to receive data on fd: " << sockfd<<" tid: "<<id << endl;
}

int main() {
    int port = 22226;
    const char *ip = "127.0.0.1";
    int backlog = 5;

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    int ret = bind(listenfd, (sockaddr *) &address, sizeof(address));
    ret = listen(listenfd, backlog);

    epoll_event events[MAX_EVENT];
    int epollfd = epoll_create(5);
    // 注意，listenfd上是不能EPOLLONESHOT事件的。
    // 否则应用程序只能处理一个客户连接!因为后续的客户连接请求不再触发listenfd上的EPOLLIN事件
    addfd(epollfd, listenfd, false);

    while (1) {
        int ret = epoll_wait(epollfd, events, MAX_EVENT, -1);
        if (ret < 0) {
            cout << "epoll failure" << endl;
            break;
        }
        for (int i = 0; i < ret; i++) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) {
                struct sockaddr_in client_address;
                socklen_t client_address_len = sizeof(client_address);
                int connfd = accept(listenfd, (sockaddr *) &client_address, &client_address_len);
                addfd(epollfd, connfd, true);// 对connfd开启oneshot
            } else if (events[i].events & EPOLLIN) {
                // 创建工作线程
                pthread_t thread;
                fds fds_for_new_worker;
                fds_for_new_worker.epollfd = epollfd;
                fds_for_new_worker.sockfd = sockfd;
                // 新启动工作线程为sockfd服务
                pthread_create(&thread, NULL, worker, (void *) &fds_for_new_worker);
            } else {
                cout << "unknown error" << endl;
            }
        }
    }
    close(listenfd);
    return 0;
}