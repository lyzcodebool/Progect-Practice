/*************************************************************************
	> File Name: list_test.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Mar 2018 06:06:49 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

#define LEN sizeof(node)

typedef struct node{
    int data;
    struct node *next;
}node, *pnode;

void insert_head(pnode *phead, int data)
{
    pnode p;
    p = (pnode)malloc(sizeof(LEN));
    if(NULL == p){
        perror("malloc fail");
        exit(1);
    }

    p->data = data;
    p->next = (*phead);
    (*phead) = p;
}

void insert_tail(pnode *phead, int data)
{
    pnode p;
    p = (pnode)malloc(sizeof(LEN));
    if(p == NULL){
        perror("malloc tail fail");
        exit(1);
    }
    p->next = NULL;
    p->data = data;

    pnode q = *phead;
    if(*phead)
    {
        while(q->next)
        {
            q = q->next;
        }
        q->next = p;
    }
}


void DeteFromHead(pnode *phead)
{
    if(*phead == NULL){
        return;
    }
    else{
        pnode p = *phead;
        *phead = (*phead)->next;
        free(p);
    }
}

void DeteFromTail(pnode *phead)
{
    pnode p, p1;
    p = (*phead);
    if(*phead)
    {
        while(p->next != NULL)
        {
            p1 = p;
            p = p->next;
        }
        if(p1)
        {
            free(p1);
            p->next = NULL;
        }
        else
        {
            free(p);
        }
    }
}

/*根据key找数据*/
void SearchByKey(pnode *phead, int key)
{
    pnode p;
    p = *phead;
    while(p != NULL && p->data != key)
    {
        p = p->next;
    }
    if(p)
    {
        printf("the data is %d\n", p->data);
    }
    else
    {
        printf("not find\n");
    }
}

void DeteByKey(pnode *phead, int data)
{
    pnode p = *phead;
    if((*phead)->data = data){
        DeteFromHead(phead);
    }else{
        while(p->next != NULL && (p->next)->data != data){
            p = p->next;
        }
        if(p->next == NULL){
            printf("not find\n");
        }else{
            pnode q = p->next->next;
            free(p->next);
            p->next = q;
        }
    }
}

//根据节点序列号来修改数据
void ModifyData(pnode *phead, int x, int y)//x节点序 y修改的数据
{
    int i = 0;
    pnode p = *phead;
    while(p){
        if(i == x){
            p->data = y;
        }
        p = p->next;
        i++;
    }
    if(i < x)
    {
        printf("输入不合法\n");
    }
}

void PrintNode(pnode *phead)
{
    pnode p = *phead;
    while(p){
        printf("%d ", p->data);
        p = p->next;
    }
    putchar(10);
}

//判断链表有没有环(t头尾相连)
int IsListCycle(pnode *phead)
{
    pnode corrent = (*phead)->next;
    if(corrent == *phead){
        printf("有环\n");
        return 0;
    }else{
        corrent = corrent->next;
    }
    printf("无环\n");
    return 1;
}

//链表中间有环
int IsCycleInMid(pnode *phead)
{
    pnode fast = (*phead)->next;
    pnode slow = *phead;

    while(fast)
    {
        if(fast == slow){
            printf("有环\n");
            return 1;
        }else{
            fast = fast->next;
            if(!fast){
                printf("无环\n");
                return 0;
            }else{
                fast = fast->next;
                slow = slow->next;
            }
        }
    }

    printf("无环\n");
    return 0;
}


//单链表的翻转
pnode ReverseList(pnode *phead)
{
    if(*phead == NULL || (*phead)->next == NULL){
        printf("链表为空或者只有一个节点\n");
    }else{
        pnode *pnew = NULL;
        pnode cur = *phead;

        while(cur != NULL){
            pnode ptemp = cur;
            cur = cur->next;
            ptemp->next = *pnew;
            *pnew = ptemp;
        }
    }
}
