#include "iostream"
#include "unistd.h"
#include "fcntl.h"
#include "string.h"
#include "daemon.h"
#include "string"
#include "threadpool.h"

int start_id = 0;
int end_id = 100000;
int step = 20000;

struct Args{
    int fd;
    int start_id;
    int end_id;
};
pthread_mutex_t lock;


//线程task
void task(void *arg){
    Args args = *static_cast<Args*>(arg);
    int fd = args.fd;
    int start_id = args.start_id;
    int end_id = args.end_id;
    char buf[128] = {'\0'};
    snprintf(buf, 127, "start_id[ %d ], end_id[ %d ], done\n", start_id, end_id);
    write(fd, buf, strlen(buf));
}


int main(int argc, char *argv[]) {

    init_daemon();
    int fd;

    //初始化线程池配置
    const int thread_cnt = 10;
    const int queue_size = 64;

    //创建线程池
    threadpool_t *tpool = threadpool_create(thread_cnt, queue_size, 0);
    if(tpool == NULL){
        fprintf(stderr, "create thread pool failed");
        exit(1);
    }

    if(-1 == (fd = open("/tmp/daemon.log", O_CREAT|O_WRONLY|O_APPEND, 0600))){
        fprintf(stderr, "open file failed\n");
        exit(1);
    }

    int n_threads = (end_id-start_id-1)/step+1;
    int ret;
    Args args;

    for(int i = 0; i < n_threads; ++i){
        int s = i*step;
        int e = s + step;
        args.start_id = s;
        args.end_id = e;
        args.fd = fd;
        ret = threadpool_add(tpool, &task, &args, 0);
        if(ret){
            fprintf(stderr, "threadpool_add failed");
            exit(1);
        }
    }
    while(tpool->count != 0){
        sleep(1);
    }
    ret = threadpool_destroy(tpool, 0);
    if(ret){
        fprintf(stderr, "destory thread pool failed\n");
        exit(2);
    }
    close(fd);
    return 0;
}