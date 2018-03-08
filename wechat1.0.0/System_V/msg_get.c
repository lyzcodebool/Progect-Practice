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

/*消息队列的基础*/

#define ERR_EXIT(m)\
        do\
        {\
            perror(m);  \
            exit(EXIT_FAILURE);  \
        }while(0)

int main(void)
{
    int msgid;
    //msgid = msgget(1234, 0666);
    msgid = msgget(1234, 0666| IPC_CREAT| IPC_EXCL);
    //创建消息队列
    //msgid = msgget(IPC_PRIVATE, 0666| IPC_CREAT| IPC_EXCL);
    if(msgid == -1){
        ERR_EXIT("msgget");
    }
    printf("msgget success!\n");

    int ret;
    //可以用man msgctl查看函数属性通过第三个结构体获取消息队列的相关属性
    //ret  = msgctl(msgid, IPC_RMID, NULL);
    struct msqid_ds buf;
    msgctl(msgid, IPC_STAT, &buf);
    printf("MODE == %o\n", buf.msg_perm.mode);
    return 0;
}


