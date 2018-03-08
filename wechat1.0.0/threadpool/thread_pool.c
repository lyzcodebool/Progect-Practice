/*************************************************************************
	> File Name: thread_pool.h
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Feb 2018 11:01:53 PM PST
 ************************************************************************/
#include<string.h>
#include<stdio.h>
#include<errno.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include "thread_pool.h"

void *thread_routine(void *arg)
{
    struct timespec abstime;
    printf("thread 0X%0x is starting!\n", (int)pthread_self());
    threadpool_t *pool = (threadpool_t *)arg;
    while(1){
        int timeout = 0;
        condition_lock(&pool->ready);
        pool->idle_count++;
        //等待队列中有任务到来或者线程池销毁通知
        while(pool->first == NULL && !pool->quit){
            printf("thread 0X%x is waiting!\n", (int)pthread_self());
            clock_gettime(CLOCK_REALTIME, &abstime);
            //超时时间5秒:w
            //
            abstime.tv_sec+= 2;
            int status;
            status = condition_timedwait(&pool->ready, &abstime);
            if(status == ETIMEDOUT){
                printf("thread 0X%x is wait timed out!\n", (int)pthread_self());
            //超时标志置一
            timeout = 1; 
            break;
            }
            //condition_timedwait(&pool->ready);
        }
        
        //等待到条件，处于工作状态
        pool->idle_count--;
        if(pool->first != NULL){
            //从队头取出任务
            dispatch_task_t *t = pool->first;
            pool->first = t->next;
            //执行任务需要一定的时间，所以需要先解锁
            //以便生产者进程能够网队列中添加任务，
            //其他消费者进程能够进入的等待任务
            condition_unlock(&pool->ready);
            t->run(t->arg);
            free(t);
            condition_lock(&pool->ready);
        }

        //如果等到到线程池销毁通知且任务都执行完毕
        if(pool->quit && pool->first == NULL){
            pool->counter --;
            if(pool->counter == 0){
                condition_signal(&pool->ready);
            }
            //这里一定注意解锁，因为跳出循环需要解锁
            condition_unlock(&pool->ready);
            break;
        }
            //这里设置超时等待的没有任务的线程自动销毁
            if(timeout && pool->first == NULL){
            pool->counter --;
            //这里一定注意解锁，因为跳出循环需要解锁
            condition_unlock(&pool->ready);
            break;        
            }
    condition_unlock(&pool->ready);
    }
    printf("thread_pool 0X%x is exting!\n", (int)pthread_self());
    return NULL;

}

//线程池的初始化
void threadpool_init(threadpool_t *pool, int threads)
{
    //对线程池中各字段初始化
    condition_init(&pool->ready);
    pool->first = NULL;
    pool->last = NULL;
    pool->counter = 0;
    pool->idle_count = 0;
    pool->max_threads = threads;
    pool->quit = 0;
}
//往线程池中添加任务
void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg)
{
    //生成新任务
    dispatch_task_t *newtask = (dispatch_task_t *) malloc(sizeof(dispatch_task_t));
    newtask->run = run;
    newtask->arg = arg;
    newtask->next = NULL;

    condition_lock(&pool->ready);
    //将任务添加到队列
    if(pool->first == NULL){
        pool->first = newtask;
    }else{
        pool->last->next = newtask;
    }
    /*
    *这里一定要注意
        *pool->last = newtask;
        *要在else外，不然会发生段错误
        *原因是last内存覆盖
    * */
        pool->last = newtask;
    //如果有等待进程，则唤醒其中一个
    if(pool->idle_count > 0){
        condition_signal(&pool->ready);
    }else if(pool->counter < pool->max_threads){
        //如果没有等待线程，并且没有超过最大线程数,则创建一个新线程
        pthread_t tid;
        pthread_create(&tid, NULL, thread_routine, pool);
        pool->counter ++;
    }
    condition_unlock(&pool->ready);

}
//销毁线程池
void threadpool_destroy(threadpool_t *pool)
{
    if(pool->quit)
    {
        return;
    } 
    condition_lock(&pool->ready);
    pool->quit = 1;
    if(pool->counter > 0){
        if(pool->idle_count > 0){
            condition_broadcast(&pool->ready);
        }
            //处于执行状态中的线程，不会受到广播
            //线程池需要等待执行任务状态中的线程全部退出
        while(pool->counter > 0){
                condition_wait(&pool->ready);  
        }
    }
    condition_unlock(&pool->ready);
    condition_destroy(&pool->ready);
}

