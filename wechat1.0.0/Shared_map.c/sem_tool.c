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

/*获取当前信号量的权限*/
int sem_getmode(int semid)
{
    union semun su;
    struct semid_ds sem;
    su.buf = &sem;
    int ret = semctl(semid, 0, IPC_STAT, su);
    if(ret == -1){
        ERR_EXIT("sem_getmode");
    }
    printf("current permissions is %o\n", su.buf->sem_perm.mode);
    return ret;
}
/*设置当前信号量的权限*/
int sem_setmode(int semid, char *mode)
{
    union semun su;
    struct semid_ds sem;
    su.buf = &sem;
    int ret = semctl(semid, 0, IPC_STAT, su);
    if(ret == -1){
        ERR_EXIT("sem_getmode");
    }
    printf("current permissions is %o\n", su.buf->sem_perm.mode);
    sscanf(mode, "%o", (unsigned int*)&su.buf->sem_perm.mode);
    ret = semctl(semid, 0, IPC_SET, su);
    if(ret == -1){
        ERR_EXIT("sem_setmode");
    }
    printf("permissions updated ...\n");
    return ret;
}


void usage()
{
    fprintf(stderr, "[sem_tool] -c\n");
    fprintf(stderr, "[sem_tool] -d\n");
    fprintf(stderr, "[sem_tool] -p\n");
    fprintf(stderr, "[sem_tool] -v\n");
    fprintf(stderr, "[sem_tool] -s <value>\n");
    fprintf(stderr, "[sem_tool] -m <mode>\n");
    fprintf(stderr, "[sem_tool] -f\n");
    fprintf(stderr, "[sem_tool] -g\n");
}


/*创建一个信号量集*/
int sem_creat(key_t key)
{
    int semid;
    semid = semget(key, 1, IPC_CREAT| IPC_EXCL| 0666);
    if(semid == -1){
        ERR_EXIT("semget");
    }else{
        printf("creat sem successful...\n");
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
    return semid;
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
    }else{
        printf("set semval successful...\n");
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
    printf("current semval is %d\n", ret);
    return ret;
}

/*删除一个信号量集*/
int sem_dete(int semid)
{
    int ret;
    ret = semctl(semid, 0, IPC_RMID, 0);
    if(ret == -1){
        ERR_EXIT("sem_dete");
    }else{
        printf("dete sem successful...\n");
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
    printf("do one p step\n");
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
    }else{
        printf("do one v step\n");
    }

    return 0;
}
int main(int argc, char *argv[])
{
    int opt;
    opt = getopt(argc, argv, "cdpvs:fgm:");
    if(opt == '?'){
        ERR_EXIT("getopt");
    }else if(opt == -1){
        usage();
        exit(EXIT_FAILURE);
    }
    //在当前路径下生成一个key
    key_t key = ftok(".", 's');
    int semid;
    switch(opt)
    {
        case 'c':
            sem_creat(key);
            break;
        case 'd':
            semid = sem_open(key);
            sem_dete(semid);
            break;
        case 'p':
            semid = sem_open(key);
            sem_p(semid);
            sem_getval(semid);
            break;
        case 'v':
            semid = sem_open(key);
            sem_v(semid);
            sem_getval(semid);
            break;
        case 's':
            semid = sem_open(key);
            sem_setval(semid, atoi(optarg));
            break;
        case 'm':
            semid = sem_open(key);
            sem_setmode(semid, argv[2]);
            break;
        case 'f':
            semid = sem_open(key);
            sem_getmode(semid);
            break;
        case 'g':
            semid =sem_open(key);
            sem_getval(semid);
            break;
    }
    return 0;
}

