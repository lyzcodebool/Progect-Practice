/*************************************************************************
	> File Name: sem_mutex.c
	> Author: 
	> Mail: 
	> Created Time: Sun 18 Feb 2018 03:18:13 AM PST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include <semaphore.h>

#define ERR_EXIT(m)\
        do\
        {\
            perror(m);\
            exit(EXIT_FAILURE);\
        }while(0)
#define CONSUMES_CONT 1
#define PRODUCERS_COUNT 5
#define BUFFSIZE 10

int g_buffer[BUFFSIZE];
sem_t g_sem_full;
sem_t g_sem_empty;
pthread_mutex_t g_mutex;

unsigned short in = 0;
unsigned short out = 0;
unsigned short produce_id = 0;
unsigned short consume_id = 0;

pthread_t g_thread[CONSUMES_CONT + PRODUCERS_COUNT];

void *consume(void *arg)
{
    int num = *(int *)arg;
    int i;
    while(1){
        printf("%d wait_buffer_empty\n", num);
        sem_wait(&g_sem_empty);
        pthread_mutex_lock(&g_mutex);
        for(i = 0; i < BUFFSIZE; i++)
        {
            printf("%02d\n", i);
            if(g_buffer[i] == -1)
                printf("%s\n", "null");
            else
                printf("%d\n", g_buffer[i]);
            if(i == out)
                printf("\t <-consume");
            putchar(10);
        }
        consume_id = g_buffer[out];

        printf("%d begin consume product %d\n",num, consume_id);
        g_buffer[out] = -1;
        out  = (out + 1)%BUFFSIZE;
        printf("%d end consume product %d\n", num, consume_id);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_empty);
        sleep(10);
        }
    return NULL;
}
void *produce(void *arg)
{
    int num = *(int *)arg;
    int i;
    while(1){
        printf("%d wait_buffer_full\n", num);
        sem_wait(&g_sem_full);
        pthread_mutex_lock(&g_mutex);
        for(i = 0; i < BUFFSIZE; i++)
        {
            printf("%02d\n", i);
            if(g_buffer[i] == -1)
                printf("%s\n", "null");
            else
                printf("%d\n", g_buffer[i]);
            if(i == in)
                printf("\t <-produce");
            putchar(10);
        }
        printf("%d begin produce product %d\n", num, produce_id);
        g_buffer[in] = produce_id;
        in  = (in + 1)%BUFFSIZE;
        printf("%d end produce product %d\n", num,  produce_id++);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_empty);
        sleep(10);
    }
    return NULL;
}

int main(void)
{
    /*将临界区数据初始化-1*/
    int j;
    for(j = 0; j < BUFFSIZE; ++j){
        g_buffer[j] = -1;
    }
    sem_init(&g_sem_full, 0, BUFFSIZE);
    sem_init(&g_sem_empty, 0, 0);

    pthread_mutex_init(&g_mutex, NULL);

    int i;
    for(i = 0; i < CONSUMES_CONT; ++i){
        pthread_create(&g_thread[i], NULL, consume, (void *)&i);
    }
    for(i = 0; i < PRODUCERS_COUNT; ++i){
        pthread_create(&g_thread[CONSUMES_CONT + i], NULL, produce, (void *)&i);
    }
    for(i = 0; i < CONSUMES_CONT+PRODUCERS_COUNT; ++i){
        pthread_join(g_thread[i], NULL);
    }

    sem_destroy(&g_sem_full);
    sem_destroy(&g_sem_empty);
    pthread_mutex_destroy(&g_mutex);

    return 0;
}

