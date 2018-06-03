#ifndef  _TIMER_HEAP_H_
#define  _TIMER_HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

/*整体思路是定时时间，将定时时间加入到最小堆中，保持最小时间在堆顶
 *最小堆每隔1s中添加一个元素，当定时时间小于等于当前时间
 的时候，执行定时任务，将堆顶的定时删除*/

//用小根堆实现的定时器

#define HEAP_SIZE 10

typedef struct Heap_t Heap_t;
typedef struct Timer_t Timer_t;
typedef struct Timer_Ctl_t Timer_Ctl_t;
typedef void (*cb_func)(Timer_Ctl_t *);

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

struct Heap_t{
    int size;
    int buffer[HEAP_SIZE];
};

struct Timer_t{
    int expire; //定时
    cb_func cb;
};

struct Timer_Ctl_t{
    Heap_t heap;
    Timer_t timer;
};

/*初始化*/
void Timer_Init(Timer_Ctl_t *timer_ctl)
{
    timer_ctl->heap.size = 0;
    timer_ctl->timer.expire = 0;
}

/*插入数据到堆中*/
int Insert_Heap(Heap_t *heap, int value)
{
    printf("Insert_Heap\n");
    int size = heap->size;
    if(size == HEAP_SIZE){
        printf("This heap is full\n");
        return 1;
    }

    heap->size ++;
    heap->buffer[size] = value;
    if(size > 0){
        int i = size;
        int j = (i-1)/2;
        while(j >= 0 && i > 0){
            if(heap->buffer[i] >= heap->buffer[j])
                break;
            else{
                heap->buffer[i] = heap->buffer[j];
                i = j;
                j = (i - 1)/2;
            }
        }
        heap->buffer[i] = value;
    }
    return 0;
}

/*删除堆顶元素*/
int Delete_Heap(Heap_t *heap)
{
    printf("detele!\n");
    int parent, child = 1;
    if(heap->size == 0){
        printf("This Heap is Empty\n");
        return 1;
    }

    heap->size --;
    int size = heap->size;
    for(parent = 0; parent*2+1 < size; parent  = child)
    {
        child = 2*parent+1;
        if(child + 1 < size){
            if(heap->buffer[child] > heap->buffer[child+1]) 
                child++;
        }
        if(heap->buffer[size] <  heap->buffer[child]){
            break;
        }
        else
            heap->buffer[parent] = heap->buffer[size];
    }
    heap->buffer[parent] = heap->buffer[child];
    return 0;
}

int deleteh(Heap_t *heap)
{
    printf("detele!\n");
    if(heap->size == 0){
        printf("This Heap is Empty\n");
        return 1;
    }
    heap->buffer[0] = heap->buffer[heap->size-1];
    heap->size--;
    int size = heap->size;
    int parent  = 0;
    int child = 1;
    while(child + 1 <= size) 
    {
        if(heap->buffer[parent] <= heap->buffer[child] && heap->buffer[parent] <= heap->buffer[child + 1])
            break;

        if(heap->buffer[child] > heap->buffer[child+1])
            child++;
        swap(&heap->buffer[parent], &heap->buffer[child]);
        parent = child;
        child = 2*parent + 1;
    }
    return 0;
}

/*获取当前时间*/
time_t Get_Current_Time()
{
    return time(NULL);
}
/*获取定时时间*/
time_t Parse_Expir_Time(int delay)
{
    return time(NULL) + delay;
}
/*启动定时器*/
void Ev_Timer_Start(int Timeout_s, cb_func cb, Timer_Ctl_t *timer_ctl)
{
    printf("timeout_s = %d\n", Timeout_s);
    int expir = Parse_Expir_Time(Timeout_s);
    timer_ctl->timer.cb = cb;
    Insert_Heap(&timer_ctl->heap, expir);
}

/*定时器回调函数*/
void Test_CallBack_Func(Timer_Ctl_t *timer_ctl)
{
    static int timeout = 0;
    printf("hello world\n");
    timeout += 1;
    printf("time = %d\n", timeout);
    Ev_Timer_Start(timeout, Test_CallBack_Func, timer_ctl);
}

/*定时器运行*/
/*void Timer_Run(Timer_Ctl_t *timer_ctl )*/
/*{*/
/*    int size = timer_ctl->heap.size;*/
/*    if(size == 0)*/
/*        return;*/
/*    time_t now = Get_Current_Time();*/
/*    timer_ctl->timer.expire = timer_ctl->heap.buffer[0];*/
/*    printf("expir = %d\n", timer_ctl->timer.expire);*/
/*    if(timer_ctl->timer.expire == now){*/
/*        printf("now : %d\n", (int)now);*/
/*        if(timer_ctl->timer.cb)*/
/*            timer_ctl->timer.cb( timer_ctl);*/
/*        Delete_Heap(&timer_ctl->heap);*/
/*    }*/
/*    sleep(1);*/
/*}*/

#endif
