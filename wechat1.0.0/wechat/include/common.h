/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: Sun 25 Feb 2018 02:00:31 AM PST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<pwd.h>
#include<ctype.h>
#include<shadow.h>

#include<crypt.h>
#include<dirent.h>
#include<time.h>
#include<sys/epoll.h>


#define ERR_EXIT(m)    \
    do \
    {\
        perror(m);  \
        exit(EXIT_FAILURE);  \
    }while(0)

#define POOL_MAX_THREADS 3
#define MAX_EPOLL_SIZE 1024
#define MAX_CLIENTS 1024
#define BUFF_SIZE 1024

//好友结构体
struct Myfriends{
    char username[16];
};

//客户端信息
struct client_info{
    char username[16];
    int client_fd;
    struct Myfriends online_friends[100];
}client_info[MAX_CLIENTS];


typedef char bool;
#define TRUE  1
#define FALSE 0

int epollfd;

/*消息体结构*/
#define SIGNIN 2        //登录
#define REGISTER 3      //注册
#define CHATROOM 4      //群聊
#define CHATSINGLE 5    //单聊
#define REDBAGPUB 6     //红包发布


#endif
