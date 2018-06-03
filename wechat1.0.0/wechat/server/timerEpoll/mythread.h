#pragma once
#ifndef  _MY_THREAD_H_
#define  _MY_THREAD_H_

#include <stdlib.h>
#include <pthread.h>
typedef  void* (*funcallback)(void *arg1, void *arg2, void *arg3);

typedef  struct {
    funcallback callback;
    void *arg1;
    void *arg2;
    void *arg3;
}Args;

void *sub_thread_entry(void *arg)
{
    Args args;
    args.callback = ((Args *)arg)->callback;
    args.arg1 = ((Args *)arg)->arg1;
    args.arg2 = ((Args *)arg)->arg2;
    args.arg3 = ((Args *)arg)->arg3;
    
    free(arg);
    return args.callback(args.arg1, args.arg2, args.arg3);
}

int my_pthread_create(pthread_t *thread, const pthread_attr_t *attr, funcallback start_routine, void *arg1, void *arg2, void *arg3)
{
    Args *args;
    if(NULL == (args = (Args *)malloc(sizeof(Args))))
        return -1;
    args->callback = start_routine;
    args->arg1 = arg1;
    args->arg2 = arg2;
    args->arg3 = arg3;
    return pthread_create(thread, attr, sub_thread_entry, args);
}

#endif //_MY_THREAD_H_
