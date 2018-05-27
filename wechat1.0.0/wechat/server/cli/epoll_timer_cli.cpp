#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

#define PORT 5000

int main(int argc, char* argv[])
{
    int sockfd, on = 1;
    char buffer[512] = {0};
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        cout << "create socket fail" << endl;
        return -1;
    }
    cout << "succeed to create client socket fd " << sockfd  << endl;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    cout << "set socket reuse by etsockopt" << endl;

    servaddr.sin_port = htons((short)PORT);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //此处更改epoll服务器地址

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        cout << "connect error" << endl;
        return -1;
    }
    cout << "succeed to connect epoll server " << endl;

    char target[] = "The Author: tao_627@aliyun.com";
    memcpy(buffer, target, strlen(target));
    int wlen = send(sockfd, buffer, strlen(buffer), 0);
    if(wlen <= 0)
        cout << " send data to server fail " << strerror(errno) << endl;
    cout << "send data to server on success, data: [" << buffer << "]"<< endl;

    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        int rlen = recv(sockfd, buffer, sizeof(buffer), 0);
        if(rlen <= 0)
            cout << " receive data from server fail " << strerror(errno) << endl;
        cout << "receive data from server on success, data: [" << buffer << "]" << endl;
    }

    return 0;
}

