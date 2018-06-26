#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <time.h>

struct shmData{
    char ch;
};

int main()
{

    int shmid = shmget(1234, 0, 0|IPC_CREAT);
    if(shmid == -1)
        printf("shmget error\n");

    void *str;
    str = shmat(shmid, NULL, 0);
    if((void *)-1 == str)
        printf("shmat error\n");
    struct shmData *shared = (struct shmData *)str;
    shared->ch = 'a';
    
    clock_t start = clock();
    for(int i = 0; i < 10000000; ++i){
        char a = shared->ch;
    }
    clock_t end = clock();
    double time = end - start;
    printf("usetime = %f\n", time/CLOCKS_PER_SEC);
    shmdt(str);
}

