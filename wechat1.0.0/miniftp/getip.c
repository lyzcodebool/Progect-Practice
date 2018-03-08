/*************************************************************************
	> File Name: getip.c
	> Author: 
	> Mail: 
	> Created Time: Sun 25 Feb 2018 02:00:11 AM PST
 ************************************************************************/

#include"common.h"
int getlocalip(char *ip)
{
    char host[100] = {0};
    if(gethostname(host, sizeof(host)) < 0){
        ERR_EXIT("gethostname");
    }
    struct hostent *hp;
    if((hp = gethostbyname(host)) == NULL)
        ERR_EXIT("gethostbyname");
    strcpy(ip, inet_ntoa(*(struct in_addr*)hp->h_addr_list[0]));
    return 0;
}

int main(void)
{
    char host[100] = {0};
    if(gethostname(host, sizeof(host)) < 0){
        ERR_EXIT("gethostname");
    }
    struct hostent *hp;
    if((hp = gethostbyname(host)) == NULL)
        ERR_EXIT("gethostbyname");

    int i = 0;
    while(hp->h_addr_list[i] != NULL){
        printf("%s\n", inet_ntoa(*(struct in_addr*)hp->h_addr_list[i]));
        i++;
    }
    char ip[16];
    getlocalip(ip);
    printf("local ip = %s\n", ip);
    return 0;
}

