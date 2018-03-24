/*************************************************************************
	> File Name: stack_test.c
	> Author: 
	> Mail: 
	> Created Time: Thu 22 Mar 2018 07:03:04 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef int SElempType;
typedef int Status;

#define INIT_SIZE 100
#define STACKINCREMENT 10
#define ok 1
#define error 0
#define true 1
#define false 0

//顺序表
typedef struct {
    SElempType *base;
    int length; //当前线性表的长度;
    int listSize;//线性表的总长度
}SqList;

void initSqlist(SqList *l)
{
    l->base = (SElempType *)malloc(sizeof(SElempType));
    if(l->base == NULL){
        perror("malloc");
        exit(1);
    }
    l->length = 0;
    l->listSize = INIT_SIZE;
}

//栈
/*typedef struct 
{
    SElempType *base;
    SElempType *top;
    int stacksize;
}SqStack;

Status InitStack(SqStack *s)
{
    s->base = (SElempType *)malloc(sizeof(SElempType));
    if(!s->base){
        perror("malloc fail");
        exit(1);
    }
    s->top = s->base;
    s->stacksize = INIT_SIZE;

    return ok;
}

//动态压栈
Status Push(SqStack *s, int value)
{
    if((s->top-s->base) == s->stacksize){
        printf("栈满，但是我们为你开辟了新的空间\n");
        s->base = (SElempType *)realloc(s->base, (s->stacksize + STACKINCREMENT)*sizeof(SElempType));
            if(s->base == NULL){
                printf("error realloc");
                exit(1);
            }
            s->top = s->base + s->stacksize;
            s->stacksize += STACKINCREMENT;
        }
        *s->top++ = value;
}*/



