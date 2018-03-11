/*************************************************************************
	> File Name: session.c
	> Author: 
	> Mail: 
	> Created Time: Sat 10 Mar 2018 12:52:40 AM PST
 ************************************************************************/

#include"session.h"
#include"sysutil.h"
#include"common.h"
#include"thread_pool.h"
#include"dispatchertask.h"
#include"condition.h"
//程序运行整体模型
unsigned short printf_info();
void main_thread_func()
{
    //unsigned short port = printf_info();
    char ip[16] = "127.0.0.1";
    int listenfd = tcp_server(ip, 8888);
    /*建立epoll文件描述符*/
    if((epollfd = epoll_create(MAX_CLIENTS)) < 0){
        ERR_EXIT("epoll_create");
    }
    
    /*返回epoll_wait 返回的可读写等就绪事件*/
    struct epoll_event readiness_event[MAX_CLIENTS];
    for(int i = 0; i < MAX_CLIENTS; ++i){
        readiness_event[i].data.fd = -1;
        client_info[i].client_fd = -1;
    }
    
    add_epoll_fd(epollfd, listenfd, FALSE);
    threadpool_t *pool = (threadpool_t *)malloc(sizeof(threadpool_t));
    threadpool_init(pool, 6);
    while(1)
    {
        /*返回就绪事件*/
        int rdiness_event_num = epoll_wait(epollfd, readiness_event, MAX_CLIENTS, -1);
        if(rdiness_event_num < 0){
            ERR_EXIT("epoll_wait");
        }
        printf("rdiness_event_num = %d\n", rdiness_event_num);
        for(int i = 0; i <rdiness_event_num; ++i){
            int fd = readiness_event[i].data.fd;
            printf("fd = %d \t readiness_event[%d].data.fd = %d\n", fd, i, readiness_event[i].data.fd);
            if(fd == listenfd)
            {
                /*有新连接到来*/
                threadpool_add_task(pool, new_connect_task, &listenfd);
            }
            else if(readiness_event[i].events & EPOLLIN)
            {
                /*有写事件到来*/
                threadpool_add_task(pool, io_read_task, &fd);
            }
            else if(readiness_event[i].events & EPOLLOUT)
            {
                /*有读事件到来*/
                threadpool_add_task(pool, io_write_task, &fd);
            }
        }
    }

}
unsigned short printf_info()
{
    unsigned short port;
    printf_info("please input the port: \n");
    scanf("%hu", &port);
    return port;
}
