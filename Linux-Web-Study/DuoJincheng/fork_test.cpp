//
// Created by baitianyu on 5/19/24.
//
#include "iostream"
#include "sys/types.h"
#include "unistd.h"
using namespace std;

int main(){

    pid_t pid = fork();

    if (pid>0){
        cout<<"pid: "<<pid<<endl;
        // > 0 是创建的子进程的进程号，= 0 代表子进程
        // is current parent process
        cout<<"now is parent process, pid:"<<getpid()<<" ppid:"<<getppid()<<endl;
    }else if (pid==0){
        // is current child process
        cout<<"now is child process, pid:"<<getpid()<<" ppid:"<<getppid()<<endl;
    }else{
        cout<<"fork failed"<<endl;
    }

    for (int i =0;i<3;i++){
        cout<<i<<endl;
        sleep(1);
    }

}