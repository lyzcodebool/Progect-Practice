/*************************************************************************
	> File Name: mmap_write.c
	> Author: 
	> Mail: 
	> Created Time: Sun 04 Feb 2018 02:10:06 AM PST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<error.h>
#include<fcntl.h>
#include<string.h>
#include <sys/mman.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include <sys/sem.h>

typedef struct stu{
    char name[32];
    int age;
}STU;

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
};


/*struct sembuf{
    short sem_num;//信号量的编号
    short sem_op; //信号量执行的操作
    short sem_flg; //两个取值是IPC_NOWAIT或者SEM_UNDO
};
*/
#define ERR_EXIT(m)\
        do\
        {\
            perror(m);\
            exit(EXIT_FAILURE);\
        }while(0)

/*创建一个信号量集*/
int sem_creat(key_t key)
{
    int semid;
    semid = semget(key, 1, IPC_CREAT| IPC_EXCL| 0666);
    if(semid == -1){
        ERR_EXIT("semget");
    }
    return semid;
}
/*打开一个信号量集*/
int sem_open(key_t key){
    int semid;
    semid = semget(key, 0, 0);
    if(semid == -1){
        ERR_EXIT("semget");
    }
    return 0;
}

/*设置信号量*/
int sem_setval(int semid, int val)
{
    union semun su;
    su.val = val;
    int ret; 
    ret = semctl(semid, 0, SETVAL, su);
    if(ret == -1){
        ERR_EXIT("semctl_setval");
    }

    return 0;
}

/*得到信号量值*/
int sem_getval(int semid)
{
    int ret; 
    ret = semctl(semid, 0, GETVAL, 0);
    if(ret == -1){
        ERR_EXIT("semctl_getval");
    }

    return ret;
}

/*删除一个信号量集*/
int sem_dete(int semid)
{
    int ret;
    ret = semctl(semid, 0,IPC_RMID, 0);
    if(ret == -1){
        ERR_EXIT("sem_dete");
    }
    return 0;
}

/*P操作*/
int sem_p(int semid)
{
    struct sembuf sb = {0, -1, 0};//表示从信号量集的第一个信号量进行p操作
    int ret;
    ret = semop(semid, &sb, 1);
    if(ret == -1){
        ERR_EXIT("semop_p");
    }

    return 0;
}

/*V操作*/
int sem_v(int semid)
{
    struct sembuf sb = {0, +1, 0};//表示从信号量集的第一个信号量进行p操作
    int ret;
    ret = semop(semid, &sb, 1);
    if(ret == -1){
        ERR_EXIT("semop_v");
    }

    return 0;
}
int main(int argc, char *argv[])
{
    int semid;
    semid = sem_creat(1234);
    sleep(10);
    sem_dete(semid);
    return 0;
}

