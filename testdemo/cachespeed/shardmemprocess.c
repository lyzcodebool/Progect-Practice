#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <time.h>
#include <pthread.h>

struct shmData{
    int ch;
};

void *write_task(void *arg)
{
    struct shmData *sh = (struct shmData *)arg;
    sh->ch += 1;
    printf("%lx Thread Write A ch Is %d\n_________________________\n", pthread_self(), sh->ch);
    printf("%lx Thread Get A ch Is %d\n", pthread_self(), sh->ch);
    return (void *)0;
}

void *read_task(void *arg)
{
    struct shmData *sh = (struct shmData *)arg;
    printf("%lx Thread Get A ch Is %d\n", pthread_self(), sh->ch);
    return (void *)0;
}
int main()
{

    pthread_t tid;
    int shmid = shmget(1234, 0, 0|IPC_CREAT);
    if(shmid == -1)
        printf("shmget error\n");

    void *str;
    str = shmat(shmid, NULL, 0);
    if((void *)-1 == str)
        printf("shmat error\n");
    struct shmData *shared = (struct shmData *)str;
    shared->ch = 1;
    for(int i = 0; i < 20; ++i){
        pthread_create(&tid, NULL, write_task, (void *)shared);
    }
    /* for(int i = 0; i < 20; ++i){ */
    /*     pthread_create(&tid, NULL, read_task, (void *)shared); */
    /* } */
    for(int i = 0; i < 40; ++i){
        pthread_join(tid, NULL);
    }
    shmdt(str);
}

