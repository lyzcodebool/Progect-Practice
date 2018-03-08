/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Feb 2018 12:53:09 AM PST
 ************************************************************************/

#include"thread_pool.h"
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void *dispatch_task(void *arg)
{
    printf("thread 0X%x working on task: %d\n", (int)pthread_self(), *(int *)arg);
    sleep(1);
    free(arg);
    return NULL;
}
int main(void)
{
    threadpool_t pool;
    threadpool_init(&pool, 3);
    int i;
    for(i = 0; i < 10; ++i){
        int *arg = (int *)malloc(sizeof(int));
        *arg = i;
        threadpool_add_task(&pool, dispatch_task, arg); 
    }
    //sleep(15);
    threadpool_destroy(&pool);
    return 0;
}
