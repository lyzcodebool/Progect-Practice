#include <stdio.h>

#include  <unistd.h>
#include  <sys/types.h>       /* basic system data types */
#include  <sys/socket.h>      /* basic socket definitions */
#include  <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include  <arpa/inet.h>       /* inet(3) functions */
#include <sys/epoll.h> /* epoll function */
#include <fcntl.h>     /* nonblocking */
#include <sys/resource.h> /*setrlimit */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

typedef char Bool;
#define true 1
#define false 0

//服务器测试代码
/* epoll ET和LT模式在socket描述符阻塞和非阻塞情况下的区别 */

#define MAXEPOLLSIZE 10000
#define MAXLINE 10
/* int setnonblocking(int sockfd) */
/* { */
/*     if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) { */
/*         return -1; */
/*     } */
/*     return 0; */
/* } */
int setnonblocking(int sockfd)
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

void addfd( int epollfd, int fd, Bool oneshot )
{
    struct epoll_event event;
    event.data.fd = fd;
    /* event.events = EPOLLIN | EPOLLET; */
    if( oneshot )
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    /* setnonblocking( fd ); */
}

void reset_oneshot( int epollfd, int fd )
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
}


static void signal_handle(int signo)
{
    switch(signo)
    {
    case SIGTERM:
        printf("SIGTERM HANDLE\n");
        break;
    default:
        printf("other signals\n");
        break;
    }
}

int handle(int connfd);
int handle_block(int connfd, int epollfd);

int main(int argc, char **argv)
{
    signal(SIGTERM, signal_handle);
    int  servPort = 6888;
    int listenq = 3;

    int listenfd, connfd, kdpfd, nfds, n, curfds,acceptCount = 0;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];
    struct rlimit rt;
    char buf[MAXLINE];

    /* 设置每个进程允许打开的最大文件数 */
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1){
        perror("setrlimit error");
        return -1;
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (servPort);

    listenfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (listenfd == -1) {
        perror("can't create socket file");
        return -1;
    }

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (setnonblocking(listenfd) < 0) {
        perror("setnonblock error");
    }

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr)) == -1){
        perror("bind error");
        return -1;
    } 
    if (listen(listenfd, listenq) == -1){
        perror("listen error");
        return -1;
    }
    /* while(1){}  //用来检测listen的两个队列 */
    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
    kdpfd = epoll_create(MAXEPOLLSIZE);
    //ev.events = EPOLLIN | EPOLLET;
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listenfd, &ev) < 0){
        fprintf(stderr, "epoll set insertion error: fd=%d\n", listenfd);
        return -1;
    }
    curfds = 1;

    printf("epollserver startup,port %d, max connection is %d, backlog is %d\n", servPort, MAXEPOLLSIZE, listenq);

    for (;;) {
        /* 等待有事件发生 */
        nfds = epoll_wait(kdpfd, events, curfds, -1);
        if (nfds == -1){
            perror("epoll_wait");
            continue;
        }
        /* 处理所有事件 */
        for (n = 0; n < nfds; ++n){
            if (events[n].data.fd == listenfd){
                connfd = accept(listenfd, (struct sockaddr *)&cliaddr,&socklen);
                if (connfd < 0){
                    perror("accept error");
                    continue;
                }
                /* addfd(kdpfd, connfd, true); */

                sprintf(buf, "accept form %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
                printf("%d:%s", ++acceptCount, buf);

                if (curfds >= MAXEPOLLSIZE){
                    fprintf(stderr, "too many connection, more than %d\n", MAXEPOLLSIZE);
                    close(connfd);
                    continue;
                } 
                /* 将连接socket设置为非阻塞 */
                /* if (setnonblocking(connfd) < 0){ */
                /*     perror("setnonblocking error"); */
                /* } */
                /* ev.events = EPOLLIN | EPOLLET;//采用ET边沿触发模式 */
                ev.data.fd = connfd;
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, connfd, &ev) < 0){
                    fprintf(stderr, "add socket '%d' to epoll failed: %s\n", connfd, strerror(errno));
                    return -1;
                }
                curfds++;
                continue;
            } 
            // 处理客户端请求
            /* if (handle(events[n].data.fd) < 0){ */
            /*     epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev); */
            /*     curfds--; */
            /* } */
            if (handle_block(events[n].data.fd, kdpfd) < 0){
                epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev);
                curfds--;
            }
        }
    }
    close(listenfd);
    return 0;
}
int handle_block(int connfd, int epollfd)
{
    char buf[1024];
    bzero(buf, sizeof(buf));
    while(1)
    {
        int nread = read(connfd, buf, sizeof(buf));
        if(nread == -1){
            /* if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR){ */
            /*     reset_oneshot(epollfd, connfd); */
            /*     printf("read later"); */
            /*     continue; */
            /* }else */ 
                return -1;
        }

        else if(nread == 0){
            printf("client close!\n");
            return -1;
        }else{
            buf[nread] = '\0';
            printf("recv: %s \n", buf);
            int nwrite = write(connfd, buf, sizeof(buf));
            if(nwrite == -1)
                return -1;
        }
    }
}
int handle(int connfd){
    int reRead = 1;
    while(reRead){
        int nread;
        char buf[MAXLINE];
        nread = read(connfd, buf, MAXLINE);//读取客户端socket流

        if (nread == 0) {
            printf("client close the connection\n");
            close(connfd);
            return -1;
        } 
        else if (nread < 0) {
            if(nread == EAGAIN)
                break;
            perror("read error");
            close(connfd);
            return -1;
        }else{
            if(nread == MAXLINE){
                write(STDOUT_FILENO, buf, nread);
                write(connfd, buf, nread+1);//响应客户端  
                reRead = 1;
            }
            else{
                write(STDOUT_FILENO, buf, nread);
                write(connfd, buf, nread);//响应客户端  
                reRead = 0;
            }
        }
    }
        return 0;
}
