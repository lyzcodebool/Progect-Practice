#include "http.h"

int main(int argc, char **argv)
{
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0)
        ERR_EXIT("socket");
    struct sockaddr_in seraddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(8888);

    int ret = connect(listenfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
    if(ret < 0)
        ERR_EXIT("connect");

    char httprequest[1024];
    char result[1024];
    sprintf(httprequest, "GET http://baidu.com HTTP/1.1\r\nhost: 127.0.0.1\r\n");
    while(1)
    {
        int ret = write(listenfd, httprequest, sizeof(httprequest));
        if(ret < 0)
            ERR_EXIT("write");
        memset(result, '\0', sizeof(result));
        int nread = read(listenfd, result, sizeof(result));
        if(nread == 0){
            printf("server close\n");
            close(listenfd);
            return 0;
        }else if(nread < 0){
            ERR_EXIT("read");
        }else{
            printf("result: %s\n", result);
        }
    }

    close(listenfd);
    return 0;
}
