/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: Sun 25 Feb 2018 02:00:31 AM PST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<pwd.h>
#include<ctype.h>
#include<shadow.h>
#include<crypt.h>
#include<dirent.h>
#include<time.h>


#define ERR_EXIT(m)    \
    do \
    {\
        perror(m);  \
        exit(EXIT_FAILURE);  \
    }while(0)

#define MAX_COMMAND_LINE 1024
#define MAX_CMMAND 32
#define MAX_ARG 1024

#define MINIFTP_CONF "miniftp.conf"
#endif
