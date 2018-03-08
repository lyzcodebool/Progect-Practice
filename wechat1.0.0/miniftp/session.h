/*************************************************************************
	> File Name: session.h
	> Author: 
	> Mail: 
	> Created Time: Tue 27 Feb 2018 02:45:19 AM PST
 ************************************************************************/

#ifndef _SESSION_H
#define _SESSION_H
#include "common.h"

typedef struct session
{
    //控制连接
    uid_t uid;
    int ctrl_fd;                    //已连接套接字
    char cmdline[MAX_COMMAND_LINE]; //命令行
    char cmd[MAX_CMMAND];           //命令
    char arg[MAX_ARG];              //参数
    
    //父子进程通道   
    int parent_fd;
    int child_fd;

    //FTP协议状态
    int  is_ascii;
    //数据连接
    struct sockaddr_in *port_addr;
    int data_fd;

}session_t;
void begin_session(session_t *sess);

#endif
