/*************************************************************************
	> File Name: sysutil.h
	> Author: 
	> Mail: 
	> Created Time: Tue 27 Feb 2018 01:18:18 AM PST
 ************************************************************************/

#ifndef _SYSUTIL_H
#define _SYSUTIL_H
#include"common.h"
int getlocalip(char *ip);
int tcp_client(unsigned int port);

int tcp_server(const char *host,  unsigned short port);
void activate_nonblock(int fd);
void deactivate_nonblock(int fd);

int read_timeout(int fd,  unsigned int wait_seconds);
int write_timeout(int fd,  unsigned int wait_seconds);
int accept_timeout(int fd, struct sockaddr_in *addr,unsigned int wait_seconds);
int connect_timeout(int fd, struct sockaddr_in *addr,unsigned int wait_seconds);

ssize_t readn(int fd, void *buf, size_t count);
ssize_t writen(int fd, void *buf, size_t count);
ssize_t recv_peek(int sockfd, void *buf, size_t len);
ssize_t readline(int sockfd, void *buf, size_t maxline);

void send_fd(int sock_fd, int fd);
void recv_fd(int recv_fd, int fd);

void add_epoll_fd(int epollfd, int fd, bool oneshot);
void resetoneshot(int epollfd, int fd);
void read_ctl_mod(int epollfd, int fd, bool oneshot);
#endif
