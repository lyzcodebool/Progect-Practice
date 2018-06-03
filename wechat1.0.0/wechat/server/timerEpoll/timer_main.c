#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "epollHeadTimer.h"
/* #include "mythread.h" */

int main(void)
{
    struct timeval now_time;
    struct timeval heap_time;
    heap_t *heap_a = (heap_t *)malloc(sizeof(heap_t));
    timerHead_t *timer = (timerHead_t *)malloc(sizeof(timerHead_t));
    int iCount = 0;
     while(1){
       sleep(1);
        TimerCallBack(timer, heap_a);
        gettimeofday(&now_time, NULL);
        heap_time.tv_sec = getHeapTop(heap_a);
        if(heap_time.tv_sec){
            printf("时间：%lu\n", heap_time.tv_sec);
            printf("当前时间：%lu\n", now_time.tv_sec);
            printf("out of time\n");
            int a = heap_time.tv_sec - now_time.tv_sec; 
            if( a <= 0){
                iCount ++;
                printf("1-----------------------------\n");
                printf("%d 次超时\n", iCount);
                deleteHeapTop(heap_a);
            }
        }
    }
    free(timer);
    free(heap_a);

    return 0;
}
