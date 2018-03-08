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

/*消息队列的基础*/

#define ERR_EXIT(m)\
        do\
        {\
            perror(m);  \
            exit(EXIT_FAILURE);  \
        }while(0)

#define MSGMAX 8192


//下面的结构体必须迁移过来，不然在结构体成员赋值的时候报错
struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[MSGMAX];    /* message data */              
};
void echo_srv(int msgid){
    int n;
    struct msgbuf msg;
    memset(&msg, 0, sizeof(msg));
    while(1){
        if((n = msgrcv(msgid, &msg, MSGMAX, 1, 0)) < 0){
            ERR_EXIT("msgrcv");
        }
        int pid;
        pid = *((int *)msg.mtext);
        fputs(msg.mtext+4, stdout);
        msg.mtype = pid;
        if(msgsnd(msgid, &msg, n, 0) < 0){
            ERR_EXIT("msgsnd");
        }
    }

}

int main(int argc, char *argv[])
{
    int msgid;
    msgid = msgget(5678, IPC_CREAT|0666);
    if(msgid < 0){
        ERR_EXIT("msgget");
    }
    echo_srv(msgid);
    return 0;
}


