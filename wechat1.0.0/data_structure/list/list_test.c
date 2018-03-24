/*************************************************************************
	> File Name: list_test.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Mar 2018 06:06:49 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
typedef char bool;
#define true 1
#define false 0

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

//找到倒数第k个节点
pnode  findRangeNode(pnode *phead, int k)
{
    if(k == 1 || *phead == NULL){
        printf("输入不合法\n");
        return NULL;
    }

    pnode pAhead;
    pnode pBehind;
    //前面的指针先跑到正向k节点
    while(k > 1 && pAhead != NULL){
        pAhead = pAhead->next;
        k--;
    }
    if(k > 1 && pAhead == NULL) //节点个数小于K，返回null
        return NULL; 
    while(pAhead->next != NULL){ //前后两个指针一起向前走，直到前面的指针到达链表末尾
        pAhead = pAhead->next;
        pBehind = pBehind->next;
    }
    
    return pBehind;
}

//查找单链表的中间节点
pnode FindMidNode(pnode *phead)
{
    if(*phead == NULL || (*phead)->next == NULL)
        return NULL;
    //设置两个指针，同上一个api的实现思想一样
    //不过pAhead一次走两步
    pnode pAhead;
    pnode pBehind;
    while(pAhead->next != NULL){
        pAhead = pAhead->next;
        pBehind = pBehind->next;
        if(pAhead->next != NULL){
            pAhead = pAhead->next;
        }
    }
    return pBehind;
}


//从尾到头打印链表
//可以使用递归也可以使用栈的先进后出特性
void PrintFromTail(pnode phead)
{
    if(phead == NULL){
        return;
    }else{
        PrintFromTail((phead)->next);
        printf("%d->", (phead)->data);
    }
    putchar(10);
}

//将两个有序链表合并成一个有序链表
pnode MergeSortList(pnode phead1, pnode phead2)
{
    if(phead1 == NULL)
        return phead2;
    if(phead2 == NULL)
        return phead1;
    pnode ListMerge = NULL;
    if(phead1->data < phead2->data){
        ListMerge = phead1;
        ListMerge->next == NULL;
        phead1 = phead1->next;
    }else{
        ListMerge = phead2;
        ListMerge->next = NULL;
        phead2 = phead2->next;
    }
    
    pnode pTemp = ListMerge;
    while(phead1 != NULL && phead2 != NULL){
        if(phead1->data < phead2->data){
            pTemp->next = phead1;
            phead1 = phead1->next;
            pTemp = pTemp->next;
            pTemp->next = NULL;
        }else{
            pTemp->next = phead2;
            phead2 = phead2->next;
            pTemp = pTemp->next;
            pTemp->next = NULL;
        }   
    }
    if(phead1 != NULL)
        ListMerge->next = phead1;
    if(phead2 != NULL)
        ListMerge->next = phead2;

    return ListMerge;
}


//合并两个有序链表成一个有序链表 
pnode MergeByRecursion(pnode phead1, pnode phead2)
{
    if(phead1 == NULL)
        return phead2;
    if(phead2 == NULL)
        return phead1;
    pnode MergeList = NULL;
    if(phead1->data < phead2->data){
        MergeList = phead1;
        MergeList->next = MergeSortList(phead1->next, phead2);
    }else{
        MergeList = phead2;
        MergeList->next = MergeSortList(phead2, phead1->next);
    }

    return MergeList;
}

//判断两个链表是否相交
//如果相交在交点以后的元素都相同，所以直比较末尾的节点是否相等，地址。
bool IsIntersec(pnode phead1, pnode phead2)
{
    if(phead1 == NULL || phead2 == NULL)
        return false;
    pnode Tail1 = phead1;
    while(Tail1->next != NULL){
        Tail1 = Tail1->next;
    }
    pnode Tail2 = phead2;
    while(Tail2->next != NULL){
        Tail2 = Tail2->next;
    }

    return Tail1 == Tail2;
}


