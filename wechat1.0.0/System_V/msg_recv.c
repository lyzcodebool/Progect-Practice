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
    char mtext[1];    /* message data */              
};

int main(int argc, char *argv[])
{
    int msgid;
    int opt;
    int flag = 0;
    int type = 0;
    while(1){
        /*getopt 用来指定终端输入的参数*/
        opt = getopt(argc, argv, "nt:");
        if(opt == '?'){
            exit(EXIT_FAILURE);
        }
        if(opt == -1)break;
        switch(opt){
            case 'n': 
                flag |= IPC_NOWAIT;
                break;
            case 't':
                type = atoi(optarg);
                break;
        }
    }
    msgid = msgget(1234, 0);
    if(msgid < 0){
        exit(EXIT_FAILURE);
    }
    struct msgbuf *ptr;

    /*
    *msgtype == 0接收第一条消息
    *msgtype > 0接收指定的消息类型
    *msgtype < 0接收<=绝对值的最小的那个开始
    */

    ptr = (struct msgbuf *)malloc(sizeof(long) + MSGMAX);
    ptr->mtype = type;
    int rcv_bytes = 0;
    if((rcv_bytes = msgrcv(msgid, ptr, MSGMAX, type, flag)) < 0){
        exit(EXIT_FAILURE);
    }
    printf("read %d batys  type = %ld\n", rcv_bytes, ptr->mtype);
    return 0;
}


