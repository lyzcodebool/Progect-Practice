#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>
//中间枢纽获得A客户端的外网ip和port发送给客户端B，获得客户端B的外网ip和port发送给A
//B通过A打的洞发数据给A，这时候A拒收B的消息，因为A的nat映射中没有B的信息，但是这次通
//信却在B的网管中添加了映射可以接受A的
//消息，同理A通过B打的洞发数据给B，这时候由于B可以接受A的消息，所以数据接收成功且在A
//的映射中加入了B的信息，从而A与B可以跨服务器通信。实现p2p

#define ERR_EXIT(m)\
    do{\
        perror(m);\
        exit(1);\
    }while(0)

/* 用来记录客户端发送过来的外网ip+port */
typedef struct{
    struct in_addr ip;
    int port;
}clientInfo;

int main()
{
    /* 一个客户端信息结构体数组，分别存放两个客户端的外网ip+port */
    clientInfo info[2];
    /* 作为心跳包需要接收的一个字节 */
    char ch; 

    /* udp socket描述符 */
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
        ERR_EXIT("SOCKET");

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(8888);
    serveraddr.sin_family = AF_INET;    

    int ret = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(ret == -1)
        ERR_EXIT("BIND");

    /* 服务器接收客户端发来的消息并转发 */
    while(1)
    {
        bzero(info, sizeof(clientInfo)*2);
        /* 接收两个心跳包并记录其与此链接的ip+port */
        socklen_t addrlen = sizeof(sockaddr_in);
        recvfrom(sockfd, &ch, sizeof(ch), 0, (struct sockaddr *)&serveraddr, &addrlen);
        memcpy(&info[0].ip, &serveraddr.sin_addr, sizeof(struct in_addr));
        info[0].port = serveraddr.sin_port;

        printf("%s \t%d creat link OK!\n", inet_ntoa(info[0].ip), ntohs(info[0].port));

        recvfrom(sockfd, &ch, sizeof(ch), 0, (struct sockaddr *)&serveraddr, &addrlen);
        memcpy(&info[1].ip, &serveraddr.sin_addr, sizeof(struct in_addr));
        info[1].port = serveraddr.sin_port;

        printf("%s \t%d creat link OK!\n", inet_ntoa(info[1].ip), ntohs(info[1].port));

        /* 分别向两个客户端发送对方的外网ip+port */
        serveraddr.sin_addr = info[0].ip;
        serveraddr.sin_port = info[0].port;
        sendto(sockfd, &info[1], sizeof(clientInfo), 0, (struct sockaddr *)&serveraddr, addrlen);

        serveraddr.sin_addr = info[1].ip;
        serveraddr.sin_port = info[1].port;
        sendto(sockfd, &info[0], sizeof(clientInfo), 0, (struct sockaddr *)&serveraddr, addrlen);
    }
    return 0;
}

