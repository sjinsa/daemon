//
// Created by songxiangyu on 2018/3/6.
//

#include "daemon.h"
#include "stdio.h"
#include "sys/stat.h"
void init_daemon(void) {
    int i = 0;
    pid_t pid;
    if((pid = fork()) < 0){
        fprintf(stderr, "fork failed");
        exit(1);
    }else if(pid > 0){
        exit(0); //父进程退出
    }

    setsid(); //子进程创建新的会话，且该子进程为该会话的首进程
    signal(SIGINT, SIG_IGN); //对终端终端符采用忽略的操作

    struct sigaction sig;
    sig.sa_handler = SIG_IGN;
    sig.sa_flags = 0;

    //对特定信号采用忽略处理
    sigaction(SIGINT, &sig, NULL);
    sigaction(SIGHUP, &sig, NULL);
    sigaction(SIGQUIT, &sig, NULL);
    sigaction(SIGPIPE, &sig, NULL);
    sigaction(SIGTTIN, &sig, NULL);
    sigaction(SIGTTOU, &sig, NULL);
    sigaction(SIGTERM, &sig, NULL);

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