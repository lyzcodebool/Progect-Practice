/*************************************************************************
	> File Name: thread_pool.h
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Feb 2018 11:01:53 PM PST
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include "condition.h"

//任务结构体，将任务放入队列由线程池中的线程来执行
typedef struct task
{
    void *(*run)(void *arg); //任务回调函数
    void *arg;               //回调函数参数
    struct task *next; 

}dispatch_task_t;

//线程池结构体
typedef struct threadpool
{
    condition_t ready;       //任务准备就绪或者线程池销毁通知
    dispatch_task_t *first;  //任务队列头指针
    dispatch_task_t *last;   //任务队列尾指针
    int counter;             //线程池中当前线程数
    int idle_count;          //当前线程正在等待任务的线程数
    int max_threads;         //线程池中允许的最大线程数
    int quit;                //销毁线程的时候置1
}threadpool_t;

//线程池的初始化
void threadpool_init(threadpool_t *pool, int threads);
//往线程池中添加任务
void threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg);
//销毁线程池
void threadpool_destroy(threadpool_t *pool);

#endif
