#include <stdio.h>
#include        <netinet/in.h>
#include        <errno.h>
#include        <stdio.h>
#include        <stdlib.h>
#include        <sys/socket.h>
#include        <arpa/inet.h>
#define MAXLINE                  4096
#define LISTENQ                  1024    /* 2nd argument to listen() */
#define SERV_PORT                9877
#define SA      struct sockaddr
#include    <string.h>


static int sockfd;
void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen, char *mesg);
void GetList(int sockfd, SA *pcliaddr, socklen_t clilen) ;

int main(int argc, char ** argv) {

    struct sockaddr_in servaddr, cliaddr;

    //使用socket()函数生成套接字文件描述符
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket error.\n");
        exit(1);
    }

    //设置服务器地址和侦听端口，初始化要绑定的网络地址结构
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;                      //地址类型为AF_INET
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);       //任意本地地址
    servaddr.sin_port = htons(SERV_PORT);               //服务器端口

    //绑定侦听端口
    bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

    char instruction[3][10] = {
        "getlist\n",
        "download\n",
        "upload\n"
    };

    int op = 0;
    int i = 0,j = 0;
    int n = 0;
    char mesg[MAXLINE];
    socklen_t len = sizeof(cliaddr);
    //printf("running here!\n");
    while(1){
        //len = clilen;
        memset(mesg,0,sizeof(mesg));
        //recvfrom接收客户端的网络数据
        n = recvfrom(sockfd, mesg, MAXLINE, 0, (SA *) &cliaddr,  &len);
        op = 0;
        //处理接受到的回车符
        for(j=0;j<=n;j++){
            if(mesg[j] == '\n') mesg[j] = '\0';
            break;
        }
        for(i = 1;i<=3;i++)
            if(strcmp(mesg,instruction[i-1])== 0){
                op = i;
                break;
            }   //choose operation
        for(j=0;j<=n;j++){
            if(mesg[j] == '\0') mesg[j] = '\n'; 
            break;
        }
        printf("op = %d\n",op);
        switch(op){
            case 1: 
                GetList(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
                break;
            //case 2:DownLoad();break;
            //case 3:UpLoad();break;
            default:{
                dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), mesg);
                break;
            }
        }
        //dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), mesg);
        //printf("echo done!\n");
    }
}
void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen, char *mesg) {
    //fputs(mesg,stdout);
    //printf("\n");
    sendto(sockfd, mesg, strlen(mesg)+1, 0, pcliaddr, clilen);
}

void GetList(int sockfd, SA *pcliaddr, socklen_t clilen) {
    printf("catch\n");

    //char mesg[MAXLINE];

    char strlin[100][MAXLINE];  //一百行缓冲区
    char sendline[MAXLINE]={};  //最终发送的字符串

    int i = 0;
    FILE *fp;


            system("ls -l>filelog.txt");    //输出重定向

            //printf("2done!\n");

            //char c;
            //int count=0;
            fp = fopen("filelog.txt", "r");

            if (!fp) {
                printf("get ls order failed!");
            } else {
                //printf("4done!\n");
                //printf("%p\n", fp);
                while (1) {
                    //printf("2done!!!!\n");
                    if (NULL==fgets(strlin[i], MAXLINE, fp))
                        break;  //如果读到文件尾，结束读取
                    //strlin[i][strlen(strlin[i] - 1)] = '\0';  //每行加\0结尾
                    i++;    //写成strlin[i++]提示i未定义，gcc的bug？？
                }
                /*
                 ++i;
                 memcpy(strlin[i],"EOF",sizeof("EOF"));
                 */
                for (int n = 0; n < i; n++){
                    strcat(sendline, strlin[n]);
                    //strcat(sendline,"\n");
                }
                //sendto(sockfd, strlin, sizeof(strlin), 0, pcliaddr, len);
                fputs(sendline,stdout);
                sendto(sockfd, sendline, strlen(sendline)+1, 0, pcliaddr, clilen);
            }
            //strcat(sendline, '\0');

            fclose(fp);

            printf("FileList Get!\n");

}

