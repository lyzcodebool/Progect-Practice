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

/*shmget共享内存的创建 
* shmat将共享内存段连接到的进程地址空间
* shmdt解除连接*/
typedef struct stu{
    char name[32];
    int age;
}STU;

#define ERR_EXIT(m)\
        do\
        {\
            perror(m);\
            exit(EXIT_FAILURE);\
        }while(0)

int main(int argc, char *argv[])
{
    int shmid;
    shmid = shmget(1234, 0, 0);
    if(shmid == -1){
        ERR_EXIT("shmid");
    }
    STU *p;
    p = shmat(shmid, NULL, 0);
    if(p == (void *)-1){
        ERR_EXIT("shmat");
    }
    printf("name = %s  age = %d\n", p->name, p->age);
    memcpy(p, "quit", 4);

    shmdt(p);
}

