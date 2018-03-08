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


//下面的结构体必须迁移过来，不然在结构体成员赋值的时候报错
struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1];    /* message data */              
};

int main(int argc, char *argv[])
{
    int msgid;
    if(argc != 3){
        fprintf(stderr, "usage: %s <bytes> <type>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int len = atoi(argv[1]);
    int type = atoi(argv[2]);

    //创建消息队列
    msgid = msgget(1234, 0);
    if(msgid == -1){
        ERR_EXIT("msgget");
    }
    printf("msgget success!\n");

    int ret;
    //可以用man msgctl查看函数属性通过第三个结构体获取消息队列的相关属性
    struct msgbuf *ptr;
    ptr = (struct msgbuf*)malloc(sizeof(long) + len);
    ptr->mtype = type;
    
    //msgsnd(msgid, ptr, len, 0);
    /*
    * 因为消息队列的能够接收的字节数有上限
    * 所以要避免字节数超过上限引起消息队列接收阻塞
    */
    /*这里可以使用非阻塞的方式*/
    if(msgsnd(msgid, ptr, len, IPC_NOWAIT) < 0){
        exit(EXIT_FAILURE);
    }

    struct msqid_ds buf;
    msgctl(msgid, IPC_STAT, &buf);
    printf("MODE == %o\n", buf.msg_perm.mode);
    printf("bytes == %lu\n", buf.__msg_cbytes);
    printf("num == %lu\n", buf.msg_qnum);
    printf("maxnum = %ld\n",buf.msg_qbytes);
    
    return 0;
}


