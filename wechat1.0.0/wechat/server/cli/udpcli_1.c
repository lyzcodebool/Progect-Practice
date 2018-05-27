#include <stdio.h>
#include        <netinet/in.h>
    #include        <errno.h>
    #include        <stdio.h>
    #include        <stdlib.h>
    #include        <sys/socket.h>
    #include        <string.h>
    #include        <arpa/inet.h>
    #define MAXLINE                  4096
    #define LISTENQ                  1024    /* 2nd argument to listen() */
    #define SERV_PORT                6888
    #define SA      struct sockaddr

    void dg_cli(FILE *, int, const SA *, socklen_t);

    int
    main(int argc, char **argv)
    {
        int sockfd;
        struct sockaddr_in servaddr;

        if (argc != 2) {
            printf("usage:udpcli01sigio <IPaddress>\n");
            exit(1);
        }

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);

        inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

        if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0) {
            printf("socket error.\n");
            exit(1);
        }

        dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));

        exit(0);
    }

    void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
    int  n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    while (fgets(sendline, MAXLINE,fp) != NULL) {
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
        n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
        recvline[n] = 0; /* null terminate */
        fputs(recvline, stdout);
        putchar(10);
    }
}
