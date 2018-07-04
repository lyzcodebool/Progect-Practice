#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>
#include <signal.h>
#include <string.h>
#define LISTENQ 5

#define ERR_EXIT(m)\
    do{\
        perror("m"); \
        exit(1); \
    }while(0)

//测试非阻塞的效果
/* 下面的写法是错误的，并不能设置非阻塞 */
/* void set_nonblock(int sockfd) */
/* { */
/*     if(fcntl(sockfd, F_SETFD, (fcntl(sockfd, F_GETFD, 0)| O_NONBLOCK))== -1) */
/*         ERR_EXIT(fcntl); */
/* } */

int open_noblock(int sockfd)
{
    int old_option;
    int new_option;

    if( (old_option = fcntl(sockfd,F_GETFL,0)) < 0)  /* 获取文件描述符旧状态标志 */
    {
        perror("fcntl GETFL");
    }
    new_option =  old_option | O_NONBLOCK;   /* 设置非阻塞标志 */
    if(fcntl(sockfd,F_SETFL,new_option) < 0)
    {
        perror("fcntl SETFL");
    }

    return old_option;  /* 返回旧文件描述符便于恢复先前状态 */
}
int ret_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
        ERR_EXIT(socket);

    struct sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    srvaddr.sin_port = htons(8888);
    srvaddr.sin_family = AF_INET;
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int ret = bind(sockfd, (struct sockaddr*)&srvaddr, sizeof(struct sockaddr_in));
    if(ret == -1)
        ERR_EXIT(bind);
    
    listen(sockfd, LISTENQ);
    socklen_t socklen = sizeof(struct sockaddr_in);
    int connfd = accept(sockfd, (struct sockaddr*)&srvaddr, &socklen);
    if(connfd == -1)
        ERR_EXIT(accept);

    return connfd;
}


int main()
{
    int connfd = ret_socket();
    /* set_nonblock(connfd); */
    open_noblock(connfd);
    char buf[1024];
    while(1)
    {
        bzero(buf, sizeof(buf));
        int ret;
        if((ret = read(connfd, buf, sizeof(buf))) < 0){
            if(errno == EWOULDBLOCK||errno == EAGAIN){
                printf("non\n");
                continue;
            }else{
                break;
            }
        }
        else if(ret == 0){
                printf("client close!\n");
                break;
        }
        else{
            buf[ret] = '\0';
            printf("%s\n", buf);
            int n = write(connfd, buf, ret);
            if(n == -1)
                ERR_EXIT(write);
        }
    }
    close(connfd);
    return 0;
}
C
