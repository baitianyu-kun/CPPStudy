//
// Created by baitianyu on 5/13/24.
//
#include "arpa/inet.h"
#include "assert.h"
#include "errno.h"
#include "fcntl.h"
#include "iostream"
#include "poll.h"
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

#define BUF_SIZE 1024
#define USER_LIMIT 12
#define FD_LIMIT 65535

struct client_data {
    sockaddr_in address;
    char *write_buff;
    char buff[BUF_SIZE];
};

int setnonblocking(int fd) {
    int old_options = fcntl(fd, F_GETFL);
    int new_options = old_options | O_NONBLOCK;// fd的状态标志
    fcntl(fd, F_SETFL, new_options);
    return old_options;
}

int main() {
    int port = 22225;
    const char *ip = "127.0.0.1";
    int backlog = 5;

    // 接收数据
    // 将客户数据发送到该服务器上的客户端

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

    // 创建users数组，分配FDLIMIT个clientdata对象，每个socket都会获得一个这样的
    // 对象，并且socket的值可以用作数组下标来进行索引clientdata
    client_data *users = new client_data[FD_LIMIT];
    pollfd fds[USER_LIMIT + 1];
    int user_counter = 0;
    // 初始化
    for (int i = 0; i <= USER_LIMIT; i++) {
        fds[i].fd = -1;
        fds[i].events = 0;
    }
    fds[0].fd = listen_fd;
    fds[0].events = POLLIN | POLLERR;
    while (1) {
        ret = poll(fds, user_counter + 1, -1);
        if (ret < 0) {
            cout << "Poll failure" << endl;
            break;
        }
        for (int i = 0; i < user_counter + 1; i++) {
            if ((fds[i].fd == listen_fd)&&(fds[i].revents&POLLIN)){
                sockaddr_in client_address;
                socklen_t client_addr_len = sizeof (client_address);
                int connfd = accept(listen_fd,(sockaddr *)&client_address,&client_addr_len);
                if (connfd<0){
                    cout<<"Accept failed, errno:"<<errno<<endl;
                    continue;
                }
                // 请求太多，关闭新的连接
                if (user_counter>=USER_LIMIT){
                    const char * info = "Too many user\n";
                    cout<<info<<endl;
                    send(connfd,info, strlen(info),0);
                    close(connfd);
                    continue;
                }
                // 对于新的连接，同时修改fds和users数组
                user_counter++;
                users[connfd].address = client_address;
                setnonblocking(connfd);
                fds[user_counter].fd = connfd;
                fds[user_counter].events = POLLIN|POLLRDHUP|POLLERR;
                cout<<"Comes a new user, total users counts: "<<user_counter<<endl;
            }else if (fds[i].revents&POLLERR){
                // 处理发生错误的情况
            }else if(fds[i].revents&POLLRDHUP){
                // 处理关闭写和关闭tcp连接的情况
            }else if (fds[i].revents&POLLIN){
                // 处理读取的情况
                int connfd = fds[i].fd;
                memset(users[connfd].buff,'\0',BUF_SIZE);
                ret = recv(connfd,users[connfd].buff,BUF_SIZE-1,0);
                cout<<"Get bytes: "<<ret<<" content: "<<users[connfd].buff<<" fd: "<<connfd<<endl;
                if (ret<0){
                    if (errno!=EAGAIN){
                        close(connfd);
                        // 将最后一个fd放到当前位置，即当前位置的关闭后，把最后一个
                        // 拿过来放到这里，然后两个都--
                        users[fds[i].fd]=users[fds[user_counter].fd];
                        fds[i]=fds[user_counter];
                        i--;
                        user_counter--;
                    }
                }else if (ret==0){

                }
                else{
                    // 如果接收到客户数据，通知其他socket连接准备写数据，群发，发给所有人
                    for (int j =1;j<=user_counter;j++){
                        if (fds[j].fd == connfd){
                            continue;
                        }
                        fds[j].events|=~POLLIN;
                        fds[j].events|=POLLOUT;
                        users[fds[j].fd].write_buff = users[connfd].buff;
                    }
                }
            }else if (fds[i].revents&POLLOUT){
                int connfd = fds[i].fd;
                if (!users[connfd].write_buff){
                    continue;
                }
                ret = send(connfd,users[connfd].write_buff, strlen(users[connfd].write_buff),0);
                users[connfd].write_buff = NULL;

                // 写完数据后重新注册fds[i]上的可读事件
                fds[i].events|=~POLLOUT;
                fds[i].events|=POLLIN;
            }
        }
    }
    delete []users;
    close(listen_fd);
    return 0;
}
