#include "http.h"

int main(int argc, char **argv)
{
    if(argc < 3){
        printf("error: [usage]<ip> <port>\n");
        exit(1);
    }
    int listenfd = getsockfd(&argv[1], &argv[2]);

    socklen_t socklen = sizeof(address);
    int conn = accept(listenfd, (struct sockaddr *)&address, &socklen);
    if(conn < 0)
        ERR_EXIT("accept");
    char buf[BUFFER_SIZE];  /*读缓冲区*/
    memset(buf, '\0', sizeof(buf));
    int data_read = 0;              
    int check_index = 0;                /*当前已经分析完了多少字节的数据*/
    int read_index = 0;                 /*当前已经读取了多少用户数据*/
    int start_line = 0;                 /*行在buffer中的起始位置*/

    while(1)
    {
        data_read = read(conn, buf, sizeof(buf));
        if(data_read < 0)
            break;
        else if(data_read == 0){
            printf("remote client has closed the connection...\n");
            break;
        }

        read_index += data_read;
        /*分析当前已经读取的所有用户数据*/
        HTTP_CODE result = parse_content(buf, check_index, read_index, start_line);
        if(result == NO_REQUEST)
            continue;
        else if(result == GET_REQUEST){
            write(conn, szret[0], sizeof(szret[0]));
            break;
        }else{
            write(conn, szret[1], sizeof(szret[1]));
            break;
        }
    }
    close(conn);
    close(listenfd);

    return 0;
}


