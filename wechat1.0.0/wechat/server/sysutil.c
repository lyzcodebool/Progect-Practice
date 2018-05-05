/*************************************************************************
	> File Name: sysutil.c
	> Author: 
	> Mail: 
	> Created Time: Mon 26 Feb 2018 10:34:07 PM PST
 ************************************************************************/

#include"../bin/sysutil.h"
#include"../bin/common.h"
void add_epoll_fd(int epollfd, int fd, bool oneshot)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET;
    if(oneshot){
        event.events |= EPOLLONESHOT;
    }
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) < 0){
        ERR_EXIT("epoll_ctl");
    }
    activate_nonblock(fd); 
}
void read_ctl_mod(int epollfd, int fd, bool oneshot)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLET|EPOLLIN;
    if(oneshot){
        event.events |= EPOLLONESHOT;
    }
    if(epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event) < 0){
        ERR_EXIT("epoll_ctl");
    }
    activate_nonblock(fd); 
}

void resetoneshot(int epollfd, int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET|EPOLLONESHOT;

    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) < 0){
        ERR_EXIT("epoll_ctl");
    }
}

int getlocalip(char *ip)
{
    char host[100] = {0};
    if(gethostname(host, sizeof(host)) < 0){
        ERR_EXIT("gethostname");        
    }
    struct hostent *hp;
    if((hp = gethostbyname(host)) == NULL)
        ERR_EXIT("gethostbyname");
    strcpy(ip, inet_ntoa(*(struct in_addr*)hp->h_addr_list[0]));
    return 0;
}

int tcp_client(unsigned int port)
{
    int sock;
    if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket client");

    struct sockaddr_in  localaddr;
    memset(&localaddr, 0, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(port);
    char ip[16] = {0};
    getlocalip(ip);
    localaddr.sin_addr.s_addr = inet_addr(ip);
    
    if(port > 0){
    int on = 1;
    if((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on))) < 0)
        ERR_EXIT("setsockopt");
    if(bind(sock, (struct sockaddr*)&localaddr, sizeof(localaddr)) < 0)
        ERR_EXIT("bind");
    }
    return sock;
}
/*
* tcp_server -- 启动一个tcp服务器
* @host:服务器的ip地址或者服务器主机名
* @port：服务器端口
* 成功返回监听套接字
*/
int tcp_server(const char *host,  unsigned short port)
{
    int listenfd;
    if((listenfd = socket(PF_INET, SOCK_STREAM, 0))< 0)
        ERR_EXIT("socket");

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    
    /*填充地址*/
    if(host != NULL){
        if(inet_aton(host, &servaddr.sin_addr) == 0){
            struct hostent *hp;
            hp = gethostbyname(host);
            if(hp == NULL)
                ERR_EXIT("gethostbyname");
            servaddr.sin_addr = *(struct in_addr*)hp->h_addr;
        }   
    }
    else{
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    /*填充端口*/
    servaddr.sin_port = htons(port);

    int on = 1;
    if((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on))) < 0)
        ERR_EXIT("setsockopt");
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("bind");
    if(listen(listenfd, SOMAXCONN) < 0)
        ERR_EXIT("listen");

    return listenfd;
}

/*
*activate_nonblock--设置I/O为非阻塞模式
*@fd：文件描述符
*/
void activate_nonblock(int fd)
{
    int ret;
    int flags = fcntl(fd, F_GETFL);
    if(flags == -1)
        ERR_EXIT("fcntl_get");

    flags |= O_NONBLOCK;
    ret = fcntl(fd, F_SETFL, flags);
    if(ret == -1)
        ERR_EXIT("fcntl_set");
}
/*
*deactivate_nonblock--设置I/O为非阻塞模式
*@fd：文件描述符
*/
void deactivate_nonblock(int fd)
{
    int ret;
    int flags = fcntl(fd, F_GETFL);
    if(flags == -1)
        ERR_EXIT("fcntl_get");

    flags &= ~O_NONBLOCK;
    ret = fcntl(fd, F_SETFL, flags);
    if(ret == -1)
        ERR_EXIT("fcntl_set");
}
/*
 *read_timeout --读超时检测函数，不含有读操作
 *@fd :文件描述符
 *@wait_seconds:等待超时秒数，如果等于0不检测超时
 *成功（没有超时）返回0， 失败返回-1， 超时返回-1并且errno = ETIMEDOUT
 */
 int read_timeout(int fd,  unsigned int wait_seconds)
 {
    int ret;
    if(wait_seconds > 0){
        fd_set read_fdset;
        struct timeval timeout;

        FD_ZERO(&read_fdset);
        FD_SET(fd, &read_fdset);

        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;

        do{
            ret = select(fd + 1, &read_fdset, NULL, NULL, &timeout);
        }while(ret < 0 && errno == EINTR);

        if(ret == 0){
            ret = -1;
            errno = ETIMEDOUT;
        }else if(ret == 1){
            ret = 0;
        }
    }
        return ret;
 }
/*
 *write_timeout -写超时检测函数，不含写操作
 *@fd :文件描述符
 *@wait_seconds:等待超时秒数，如果等于0不检测超时
 *成功（没有超时）返回0， 失败返回-1， 超时返回-1并且errno = ETIMEDOUT
 */
int write_timeout(int fd,  unsigned int wait_seconds)
 {
    int ret;
    if(wait_seconds > 0){
        fd_set write_fdset;
        struct timeval timeout;

        FD_ZERO(&write_fdset);
        FD_SET(fd, &write_fdset);

        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;

        do{
            ret = select(fd + 1, NULL, NULL, &write_fdset, &timeout);
        }while(ret < 0 && errno == EINTR);

        if(ret == 0){
            ret = -1;
            errno = ETIMEDOUT;
        }else if(ret == 1){
            ret = 0;
        }
    }
        return ret;
}

/*
 *accept_timeout -带超时的accept
 *@fd：套接字
 *@addr：输出参数，返回对方地址
 *@wait_seconds:等待超时秒数，如果等于0表示正常秒数
 *成功（未超时）返回已连接套接字， 超时返回-1并且errno = ETIMEDOUT
 */
int accept_timeout(int fd, struct sockaddr_in *addr, unsigned int wait_seconds)
{
    int ret;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if(wait_seconds > 0){
        fd_set accept_fdset;
        struct timeval timeout;

        FD_ZERO(&accept_fdset);
        FD_SET(fd, &accept_fdset);

        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;

        do{
            ret = select(fd + 1, &accept_fdset, NULL, NULL, &timeout);
        }while(ret < 0 && errno == EINTR);
        if(ret == -1){
            return -1;
        }else if(ret == 0){
            errno = ETIMEDOUT;
            return -1;
        }else if(ret == 1){
            ret = 0;
        }
    }
    if(addr != NULL)
        ret = accept(fd, (struct sockaddr*)addr, &addrlen);
    else
        ret = accept(fd, NULL, NULL);
    if(ret == -1)
        ERR_EXIT("accept");

    return ret;
}

/*
 *connect_timeout -带超时的connect
 *@fd：套接字
 *@addr：要链接的对方地址
 *@wait_seconds:等待超时秒数，如果等于0表示正常秒数
 *成功（未超时）返回已连接套接字， 超时返回-1并且errno = ETIMEDOUT
 *这里考虑到rtt系统默认的syn返回时间（75s）太长
 */
int connect_timeout(int fd, struct sockaddr_in *addr,unsigned int wait_seconds)
{
    int ret;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    if(wait_seconds > 0)
        activate_nonblock(fd);

    ret = connect(fd, (struct sockaddr*)addr, addrlen);
        
        /*设置为非阻塞如果返回失败并且errno==正在处理中...*/
    if(ret < 0 && errno == EINPROGRESS)
    {
        fd_set connect_fdset;
        struct timeval timeout;

        FD_ZERO(&connect_fdset);
        FD_SET(fd, &connect_fdset);
        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;
        
        do{
            /*一旦连接建立，套接字就可写*/
            ret = select(fd+1, NULL, &connect_fdset, NULL, &timeout);
        }while(ret < 0 && errno == EINTR);
        if(ret == 0){
            //连接超时
            ret = -1;
            errno = ETIMEDOUT;
        }else if(ret < 0){
            return -1;
        }else if(ret == 1)
        {
            /*
            *ret 返回1， 有两种情况，一种是连接建立成功，一种是套接字产生错误
            *此错误信息不能保存在errno中，因此需要调用getsockopt来获取
            */
            int err;
            socklen_t socklen = sizeof(err);
            int sockoptret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &socklen);
            if(sockoptret == -1){
                return -1;
            }
            if(err == 0){
                ret = 0;
            }else{
                errno = err;
                printf("errno = %d\n", errno);
                ret = -1;
            }
        }
    }
    if(wait_seconds > 0)
        deactivate_nonblock(fd);
    return ret;
}

/*粘包问题的解决法*/
/*readn --一次读取count个字节*/
ssize_t readn(int fd, void *buf, size_t count)
{
    size_t nleft = count; //剩余的字节数
    ssize_t nread;         //接收到的字节数
    char *bufp = (char *)buf;

    while(nleft > 0){
        if((nread = read(fd, bufp, nleft)) < 0){
            //信号中断
            if(errno == EINTR)
                continue;
            return -1;
        }else if(nread == 0){
            return (count-nleft);    
        }
        bufp += nread;
        nleft -= nread;
    }
    return count;
}
/*粘包问题的解决法*/
/*writen --一次写count个字节*/
ssize_t writen(int fd, void *buf, size_t count)
{
    size_t nleft = count; 
    ssize_t nwriten;
    char *bufp = (char *)buf;

    while(nleft > 0){
        if((nwriten = write(fd, bufp, nleft)) < 0){
            //信号中断
            if(errno == EINTR)
                continue;
            return -1;
        }else if(nwriten == 0){
            return (count-nleft);    
        }
        bufp += nwriten;
        nleft -= nwriten;
    }
    return count;
}

/*读取数据并不将套接口缓冲区中的数据移除，这里于read函数对比*/
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while(1){
        int ret = recv(sockfd, buf, len, MSG_PEEK);
        if(ret == -1 && errno == EINTR){
            continue;
        }else{
            return ret;
        }
    }
}
/*
* 按行读取
* 只能运用于套接口
*/
ssize_t readline(int sockfd, void *buf, size_t maxline)
{
    int ret;
    int nread;
    char *bufp = (char *)buf;
    int nleft = maxline;
    while(1)
    {
        ret = recv_peek(sockfd, bufp, nleft);//偷窥缓冲区
        if(ret < 0){
                return ret;
        }else if(ret == 0){
            return ret;
        }
        nread = ret;
        int i = 0;
        for(; i < nread; ++i){
            if(bufp[i] == '\n'){
                ret = readn(sockfd, bufp, i+1);//读走
                if(ret != (i+1))
                    exit(EXIT_FAILURE);
                return ret;
            }
        }
        if(nread > nleft){
            exit(EXIT_FAILURE);
        }
        nleft -= nread;
        ret = readn(sockfd, bufp, nread);
        if(ret != nread)
            exit(EXIT_FAILURE);
        bufp += nread;
    }
    return -1;
}
