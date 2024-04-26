#include <arpa/inet.h>
#include <assert.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<unistd.h>
using namespace std;

static bool stop = false;
// sigterm的处理函数，触发时结束主程序中的循环
static void handle_item(int sig) {
    stop = true;
}
int main() {
    /*
    // 大部分pc用小端，所以还是得需要转换成统一的大端来进行数据处理
    // 网络字节顺序NBO（Network Byte Order）：
    // 按从高到低的顺序存储，在网络上使用统一的网络字节顺序，可以避免兼容性问题。
    // 主机字节顺序（HBO，Host Byte Order）：
    // 不同的机器HBO不相同，与CPU设计有关，数据的顺序是由cpu决定的,
    // 而与操作系统无关。

    发送端总是把要发送的 数据转化成大端字 节序数据后再发送，
    而接收端知道对方传送过来的数据总是采用大端字 节序，
    所以接收端可以根据自身采用的字 节序决定是否对接收到的数据进行转换 (小端机转换，大端机不转换)。
    因此大端字节序也称为网络字节序，它给所有接收数据的 主机提供了 一 个正确解释收到的格式化数据的保证。
    */
    int port = 22234;
    const char *ip = "192.168.182.128";
    // backlog参数提示内核监听队列的最大长度。监听队列的长度如果超过backlog，服务器将不受理新的客户连接，客户端也将收到ECONNREFUSED错误信息
    int backlog = 5;
    // 将 SIGTERM 信号（终止信号）的处理程序设置为名为 handle_term 的函数。收到该信号后调用它
    signal(SIGTERM, handle_item);

    // 创建socket,domain 参数告诉系统使用哪个底层协议族。
    // rotocol 参数是在前两个参数构成的协议集合下，再选择一个具体的协议。
    // 不过这个值 通常都是唯一的 (前两个参数已经完全决定 了它的值)。
    // 几乎在所有情况 下，我们都应该把 它设置为0， 表示使用默认协议。
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);//fd file descriptor
    assert(sockfd >= 0);

    // TCP/IP协议族有sockaddr_in 和sockaddr_in6两个专用socket地址结构体
    struct sockaddr_in address;
    // 把address设置为0
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);// 转大端网络序
    // inet_aton(ip, &address.sin_addr); // 转大端网络序,只可用于ipv4
    inet_pton(AF_INET, ip, &address.sin_addr);// 可以用于v4和v6，只要指定一下就行

    int ret = bind(sockfd, (struct sockaddr *) &address, sizeof(address));
    assert(ret != -1);

    // 完整连接最多有(backlog+1)
    // 完整连接的上限通常比backlog 值略大。
    ret = listen(sockfd, backlog);

    while (!stop) {
        sleep(1);
    }

    close(sockfd);
    return 0;
}