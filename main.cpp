#include <iostream>
#include <stdlib.h>
#include "unistd.h"
#include "fcntl.h"
#include "string.h"
using namespace std;

void init_daemon(){
    int i = 0;
    pid_t pid;
    if((pid = fork()) < 0){
        fprintf(stderr, "fork failed");
        exit(1);
    }else if(pid > 0){
        exit(0); //父进程退出
    }

    setsid(); //子进程创建新的会话，且该子进程为该会话的首进程
    if((pid = fork()) < 0){
        fprintf(stderr, "fork failed");
        exit(1);
    }else if(pid > 0){ //防止第一个子进程创建终端
        exit(0);
    }

    umask(0); //为守护进程初始化umask
    chdir("/tmp"); //修改守护进程的工作目录
    for(; i < getdtablesize(); ++i){ //关闭所有文件描述符
        close(i);
    }
    return ;
}

int main() {
    int fd;
    time_t t;
    char buf[] = {"hello world, I'm a daemon progress\n"};
    char *datetime;
    init_daemon();

    while(1){
        if(-1 != (fd = open("/tmp/daemon.log", O_CREAT | O_WRONLY | O_APPEND, 0600))){
            fprintf(stderr, "open file[ %s ] failed", "/tmp/daemon.log");
            break;
        }
        write(fd, buf, strlen(buf));
        time(&t);
        datetime = asctime(localtime(&t));
        write(fd, datetime, strlen(datetime));
        close(fd);
        sleep(60);
    }
    return 0;
}