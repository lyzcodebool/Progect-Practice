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


#define pause_time rand()%10
int semid;
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
void wait_one_fork(int no)
{
    struct sembuf buf = {no, -1, 0};
    int ret = semop(semid, &buf, 1);
    if(ret == -1){
        ERR_EXIT("semop");
    }
}
void fork2_free(int no)
{
    int left = no;
    int right = (no+1)%5;
    struct sembuf buf[2]= {
        {left, 1, 0}, 
        {right, 1, 0}
    };
    semop(semid, buf, 2);
}
void wait_2fork(int no)
{
    int left = no;
    int right = (no+1)%5;
    struct sembuf buf[2]= {
        {left, -1, 0}, 
        {right, -1, 0}
    };
    semop(semid, buf, 2);
}
void philosophere(int no)
{
    srand(getpid());
    for(; ;){
   /*     
    *   printf("%d thinking...\n", no);
        sleep(pause_time);
        printf("%d hungry...\n", no);
        wait_2fork(no);
        printf("%d eating...\n", no);
        fork2_free(no);
        printf("%d ending...\n", no);
    */

    /*死锁情况，就是哲学家们饿了以后就拿起一个叉子*/
        int left = no;
        int right = (no + 1)%5;
        printf("%d thinking...\n", no);
        sleep(pause_time);
        printf("%d hungry...\n", no);
        wait_one_fork(left);
        printf("%d get fork\n", no);
        sleep(pause_time);
        wait_one_fork(right);
        printf("%d get fork\n", no);
        printf("%d eating...\n", no);
        fork2_free(no);
    }
}
int main(int argc, char *argv[])
{
    semid = semget(IPC_PRIVATE, 5, IPC_CREAT|0666);
    if(semid == -1){
        ERR_EXIT("semget");
    }
    union semun su;
    su.val = 1;
    int i;
    for(i = 0; i < 5; ++i){
        semctl(semid, i, SETVAL, su);
    }
    int no = 0;
    pid_t pid;
    for(i = 1; i < 5; ++i){
        pid = fork();
        if(pid == -1){
            ERR_EXIT("fork");
        }else if(pid == 0){
            no = i;
        }
    }
    philosophere(no);
}

