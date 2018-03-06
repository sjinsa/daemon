//
// Created by songxiangyu on 2018/3/6.
//

#ifndef DAEMON_DAEMON_H
#define DAEMON_DAEMON_H


#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "signal.h"


#ifdef __cplusplus
extern "C" {
#endif

//初始化守护进程
void init_daemon(void);


#ifdef __cplusplus
}
#endif

#endif //DAEMON_DAEMON_H
