#include <stdio.h>
#include "../bin/common.h"
#include "../bin/mesgcli.h"
#include "../bin/menucli_select.h"

int main(int argc, char *argv[])
{
    /* typedef struct mesgcli{ */
    /*     int sign_in;         //登录请求 */
    /*     int registered;      //注册请求 */
    /*     int chatroom;         //群聊请求 */
    /*     int chatsigle;       //单聊请求 */
    /*     int redbag_pub;      //红包发布 */
    /* }MESGCLI; */
    

    /*初始化客户端数据包*/
    MESGCLI mesgcli = {
        -1, 
        -1,
        -1,
        -1,
        -1,
    };
        
    menu_cli_select();
    int sockfd;
    socklen_t slen = sizeof(struct sockaddr);
    struct sockaddr_in seraddr;

    bzero(&seraddr,sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(0);
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);


    //socket()
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        exit(-1);
    }
    //connect()
    if(connect(sockfd,(struct sockaddr *)&seraddr,slen) == -1)
    {
        perror("connect");
        exit(-1);
    }


    return 0;
}

