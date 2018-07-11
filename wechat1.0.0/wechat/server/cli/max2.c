#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/wait.h>


#define ERR_EXIT(m)\
    do{\
        perror(m); \
        exit(1); \
    }while(0)


int main(void)
{

    int socketfd, connfd;
    int n = 0;
    /* int port = 1011; */
    while(n < 50000)
    {
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0)
        ERR_EXIT("SOCKET");
    struct ifreq ifr;
    memset(&ifr, 0x00, sizeof(ifr));
    strncpy(ifr.ifr_name, "ens33:2", strlen("ens33:2"));
    int on = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifr, sizeof(ifr));
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

    printf("socketfd = %d\n", socketfd);
    struct sockaddr_in serveraddr, cliaddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("192.168.189.139");
    serveraddr.sin_port = htons(6888);
    /* memset(&serveraddr, 0, sizeof(serveraddr)); */
    /* cliaddr.sin_family = AF_INET; */
    /* cliaddr.sin_addr.s_addr = inet_addr("172.16.1.222"); */
    /* cliaddr.sin_port = htons(port); */

    /* bind(socketfd, (struct sockaddr*)&cliaddr, sizeof(struct sockaddr_in)); */

    //无限循环建立连接，不做数据处理，建立长连接
    int opt = 1;
    int ret2 = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(ret2 < 0)
        ERR_EXIT("setsockopt");
        printf("1\n");
        socklen_t socklen = sizeof(serveraddr);
        connfd = connect(socketfd, (const struct sockaddr*)&serveraddr, socklen);
        if(connfd < 0)
            ERR_EXIT("connfd");
        n++;
        /* port++; */
    }
    while(1){}
    close(socketfd);
}
