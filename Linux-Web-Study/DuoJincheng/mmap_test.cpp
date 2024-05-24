//
// Created by baitianyu on 5/21/24.
//
#include "cstdio"
#include "cstdlib"
#include "cstring"
#include "iostream"
#include "sys/mman.h"
#include "sys/stat.h"
#include "unistd.h"
#include "wait.h"
// 将磁盘文件的数据映射到内存，用户通过修改内存就能修改硬盘文件

using namespace std;

int main() {

    // 匿名映射
    int len = 4096;
    // addr null, auto create mapping

    // shared: 内存映射区的文件会自动和磁盘文件进行同步，如果要通信的话需要设置这个
    // private：不同步，内存映射区的数据改变了，对原来的文件不影响，会重新创建一个新的文件（copy on write）
    // fork也是copy on write
    // fd为需要映射的文件的文件描述符
    // 通过open的到，open的是一个磁盘文件
    // 文件的大小不能为0，open的权限不能和port的参数有冲突
    // offset为文件的偏移量
    // 若映射成功则返回映射区的内存起始地址，否则返回MAP_FAILED(－1)，错误原因存于errno 中。
    void *ptr = mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    // 父子进程之间进行通信，先在上面创建好内存映射区之后再fork，就可以进行共享
    // 无关的进程之间共享需要准备一个大小不为0的磁盘文件，
    // 进程1通过该文件进行创建内存映射区得到指针，进程2通过该文件进行创建内存映射区得到指针，然后再通信
    // 内存映射区非阻塞，没有数据读的话会返回0
    pid_t pid = fork();
    if (pid > 0) {
        // farther
        // //10分
        // 为了实现链式操作，将目的地址返回，加3分
        //char* strcpy(char *strDest, const char *strSrc)
        //{
        //    assert( (strDest != NULL) && (strSrc != NULL) );
        //    char *address = strDest; 会先保存一下原来的地址的，
        //    方便返回原本地址的起始位置，所以内存映射区的ptr是不会变的，方便munmap进行关闭
        //    while( (*strDest++ = *strSrc++) != '\0' );
        //    retirm address;
        //}
        strcpy((char *) ptr, "hello world");
        wait(nullptr);
    } else if (pid == 0) {
        // son
        sleep(1);
        printf("%s\n", (char *) ptr);
    }

    // release
    int ret = munmap(ptr, len);

    if (ret == -1) {
        perror("munmap");
        exit(0);
    }
    return 0;
}