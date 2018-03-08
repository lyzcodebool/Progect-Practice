/*************************************************************************
	> File Name: msg_get.c
	> Author: 
	> Mail: 
	> Created Time: Thu 01 Feb 2018 02:24:10 AM PST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<getopt.h>
#include<string.h>
#include<unistd.h>

/*消息队列的基础*/

#define ERR_EXIT(m)\
        do\
        {\
            perror(m);  \
            exit(EXIT_FAILURE);  \
        }while(0)

#define MSGMAX 1024


//下面的结构体必须迁移过来，不然在结构体成员赋值的时候报错
struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[MSGMAX];    /* message data */              
};
//回射客户端
void echo_cli(int msgid)
{
    int n;
    struct msgbuf msg;
    int pid = getpid();
    memset (&msg, 0, sizeof(msg));
    *((int *)msg.mtext) = pid;
    while(fgets(msg.mtext+4, MSGMAX, stdin) != NULL){
        msg.mtype = 1;
        if(msgsnd(msgid, &msg, 4+strlen(msg.mtext+4), 0) < 0){
            ERR_EXIT("msgsnd");
        }
        memset(msg.mtext+4, 0, MSGMAX-4);
        if((n = msgrcv(msgid, &msg, MSGMAX, pid, 0)) < 0){
            ERR_EXIT("msgrcv");
        }
        fputs(msg.mtext+4, stdout);
        memset(msg.mtext+4, 0, MSGMAX-4);
    }
}

int main(int argc, char *argv[])
{
    int msgid;
    msgid = msgget(5678, 0);
    if(msgid < 0){
        ERR_EXIT("msgget");
    }
    echo_cli(msgid);
    return 0;
}


