#pragma once
#ifndef _EPOLL_HEAD_TIMER_H_
#define _EPOLL_HEAD_TIMER_H_

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <unistd.h>


#define  HEAP_SIZE 10
typedef  struct timerHead timerHead_t;
typedef  struct heap heap_t;
typedef  void (*Fun)(timerHead_t *, heap_t *);

void swap(int *number1, int *number2)
{
    int temp = *number1;
    *number1 = *number2;
    *number2 = temp;
}
typedef enum 
{
    TRUE,
    FALSE
}BOOL;

typedef struct timerHead{
    struct   timeval expire;           //超时时间
    Fun func;          //超时回调函数;
    void     *arg;                     //回调函数参数
}timerHead_t;

typedef struct clientdata{
    char            ip_address[30];    //客户端ip地址
    int             port;              //客户端端口号
    char            message[1024];     //消息
    timerHead_t     time_expir;        //过期时间
    int             flag;              //标记位，序列号
}clientdata_t;

//构建堆相关结构体和增删等操作
typedef  struct heap{
    int             size;              //堆大小
    time_t          buf[HEAP_SIZE];    //堆数据
}heap_t;

//插入数据到堆顶,我们采用最小堆
BOOL  InsertHead(heap_t *heap, time_t value)
{
    printf("insert the timeout\n");
    int size = heap->size;
    if(size == HEAP_SIZE){
        printf("timeheap cannot add more...\n");
        return FALSE;

    }
    heap->size ++;
    heap->buf[size] = value;
    
    if(size > 0){
        int i = size;
        int j = (i-1)>1;
        while(j >= 0 && i > 0){
            if(heap->buf[size] <= heap->buf[j])
                break;
            else{
                heap->buf[size] = heap->buf[j];
                i = j;
                j = (i-1)>1;
   
        
            }

        }
        heap->buf[i] = value;
    }
    return TRUE;
}

//获取最小堆的堆顶元素
time_t getHeapTop(heap_t *heap)
{
    return heap->buf[0];
}

//删除最小堆的堆顶元素
/*成功删除返回TURE， 失败返回FALSE*/

BOOL deleteHeapTop(heap_t *heap)
{
    printf("detele the top\n");
    if(heap->size == 0){
        printf("timer heap is empty...\n");
        return FALSE;
    }
    heap->buf[0] = heap->buf[heap->size-1]; //最后一个元素代替堆顶元素，然后调整为最小堆
    heap->size--;
    int size = heap->size;
    int parent = 0;
    int child = 1;
    while(child + 1 <= size){
        if(heap->buf[parent] < heap->buf[child] && heap->buf[parent] > heap->buf[child+1]) 
            break;
        if(heap->buf[child] > heap->buf[child + 1]){
            child++;
        }
        swap((int *)&heap->buf[child], (int *)&heap->buf[parent]);
        parent = child;
        child = (parent <1)|1;
    }
    return TRUE;
}

//获取当前时间
time_t getCurrTime()
{
   return time(NULL);
}

//获取定时时间
time_t parseExpirTime(timerHead_t timer, int delay)
{
   gettimeofday(&timer.expire, NULL);
   timer.expire.tv_usec += delay * 1000000;
   if(timer.expire.tv_usec >= 1000000){
       timer.expire.tv_sec += timer.expire.tv_usec/1000000; //秒数
       timer.expire.tv_usec %= 1000000;                     //微秒数,1微秒相当于百万分之一秒
   }

   return timer.expire.tv_sec;
}
//启动定时器
void EvTimerStart(timerHead_t timer, Fun callback, heap_t *heap, int delay)
{
    printf("1\n");
    struct timeval t_out;
    t_out.tv_sec = parseExpirTime(timer, delay);
    timer.func = callback;
    int ret = InsertHead(heap, t_out.tv_sec);
    if(ret == FALSE){
        usleep(1);
    }
    /* return (void *)0; */
}

//定时器回调函数
void TimerCallBack(timerHead_t *timer, heap_t *heap)
{
    static int time_out;
    printf("hello timer\n");
    time_out += 1;
    EvTimerStart(*timer, TimerCallBack, heap, time_out);
}

/* void *parseExpirTime(void *heap) */
/* { */
/*     heap_t *heap_a = (heap_t *)heap; */
/*     timerHead_t timer; */
/*     gettimeofday(&timer.expire, NULL); */
/*     timer.expire.tv_usec += 5* 1000; */
/*     if(timer.expire.tv_usec >= 1000000){ */
/*         timer.expire.tv_sec += timer.expire.tv_usec/1000000; //秒数 */
/*         timer.expire.tv_usec %= 1000000;                     //微秒数,1微秒相当于百万分之一秒 */
/*     } */
/*     return (void*)0; */
/* } */

void tick(heap_t *heap, int sockfd)
{
    //心搏函数...
    //处理过期任务,当epoll_wait超时的时候，执行该函数
    //首先我们要删除堆顶元素，自定义该函数的行为。
    
    //这里我自定义：如果epoll_wait超时，我就发送消息告知客户端，请求超时, 客户端可以选择重连，或者是断开连接。
    //我直接让它断开连接。只作为实现定时器用
    //你也可以做个超时重传类似的功能.
    deleteHeapTop(heap);

    //ack客户端...
    
    

    close(sockfd);
}

int my_epoll_wait(/*int epollfd,  epoll_event *events, int max_event_num, */heap_t *heap)
{
    timerHead_t timer;
    struct timeval now_time;
    struct timeval more_time;

    while(1)
    {
        gettimeofday(&now_time, NULL);
        timer.expire.tv_sec = getHeapTop(heap);
        more_time.tv_sec = timer.expire.tv_sec - now_time.tv_sec;
        if(more_time.tv_sec <= 0){
            more_time.tv_usec += 1000000;
            more_time.tv_sec--;
        }


                    
    }
}

#endif
