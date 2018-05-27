#include <stdio.h>
#include<stdio.h>                                                                      
#include<stdlib.h>
#include<errno.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <fcntl.h>
#include<unistd.h>
#include<string.h>

char buf[1024];
struct sockaddr_in client;
socklen_t clientlen = sizeof(client);

void Usage(const char* proc)
{
    printf("%s[ip][port]",proc);
}

int setnonblock(int udpfd)
{
    if(fcntl(udpfd, F_SETFL, fcntl(udpfd, F_GETFL, 0)|O_NONBLOCK) == -1){
        return -1;
    }
    return 0;
}
void *recvfrom_task(void *arg)
{

    memset(&buf, 0, sizeof(buf));
    int udpfd = *(int *)arg;
    int n = recvfrom(udpfd, buf, sizeof(buf), 0, (struct sockaddr*)&client, &clientlen);
    if(n < 0){
        if(errno == ECONNRESET){
            printf("read error\n");
            close(udpfd);
        }
    }else if(n == 0){
        printf("server close\n");
        close(udpfd);
    }else{
        printf("recv: %s\n", buf);
    }
    return (void *)0;
}
int main(int argc,char* argv[])
{
    if(argc!=3){
        Usage(argv[0]);
        return 1;
    }

    pthread_t tid;
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0){
        perror("socket");
        return 2;
    }
    if(setnonblock(sock) < 0){
        printf("setnonblock error\n");
        exit(1);
    }
    int _port=atoi(argv[2]);
    char* _ip=argv[1];
    client.sin_family=AF_INET;
    client.sin_port=htons(_port);
    client.sin_addr.s_addr=inet_addr(_ip);
    while(1)
    {
        ssize_t _s=read(0,buf,sizeof(buf)-1);
        if(_s>0){
            buf[_s]='\0';
        }
        _s=sendto(sock,buf,sizeof(buf)-1,0,(struct sockaddr*)&client,sizeof(client));


        int ret = pthread_create(&tid, NULL, recvfrom_task, (void *)&sock);
        if(ret < 0){
            printf("pthread creat fail!\n");
            close(sock);
            exit(1);
        }
    }
    pthread_join(tid, NULL);
    close(sock);
    return 0;
}
