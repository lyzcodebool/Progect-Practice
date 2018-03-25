/*************************************************************************
	> File Name: skip_list.c
	> Author: 
	> Mail: 
	> Created Time: Sat 24 Mar 2018 07:41:24 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#define LEVEL_MAX_LEN 10
typedef char bool;
#define true 1
#define false 2

//跳跃表节点结构体
typedef struct SkipListNode{
    int key;
    int data;
    struct SkipListNode *forward[1];
}SkipListNode;

//跳跃表链表结构
typedef struct SkipList{
    int level;
    struct SkipListNode *head;
}SkipList;

//节点初始化
SkipListNode *CreatSkipNode(int level, int key, int data)
{
    SkipListNode *newNode = (SkipListNode *)malloc(sizeof(SkipListNode) + level*sizeof(SkipListNode));
    if(newNode == NULL)
        return NULL;
    newNode->key = key;
    newNode->data = data;

    return newNode;
}

//初始化跳表
SkipList *CreatSkipList()
{
    SkipList *newlist = (SkipList *)malloc(sizeof(SkipList));
    if(newlist == NULL)
        return NULL;
    newlist->head = CreatSkipNode(LEVEL_MAX_LEN-1, 0, 0);
    for(int i = LEVEL_MAX_LEN-1; i >= 0; i--){
        newlist->head->forward[i] = NULL;
    }
    return newlist;
}

//随机产生层数
int RandLevel()
{
    int k = 1;
    while(rand()%2)
        k++;
    return (k < LEVEL_MAX_LEN)?k:LEVEL_MAX_LEN;
}

//插入节点
bool InsertNode(SkipList *sl, int key, int data)
{
    SkipListNode *update[LEVEL_MAX_LEN];
    SkipListNode *p, *q = NULL;
    p = sl->head;
    int k = sl->level;
    //从高到低找节点插入的位置，update存储每一层应该插入的位置
    for(int i = k-1; i >= 0; i--){
        while((q = p->forward[i]) && (q->key < key)){
            p = q;
        }
        update[i] = p;
    }
    //不能插入相同的key
    if(q && q->key == key){
        return false;
    }

    //产生一个随机层数
    //新建一个待插入节点q，层层插入
    k = RandLevel();
    //更新跳跃表的level
    if(k > (sl->level)){
        for(int i = (sl->level); i < k; ++i){
            update[i] = sl->head;
        }
        sl->level = k;
    }

    q = CreatSkipNode(k, key, data);
    //逐层更新节点的指针，跟普通链表的插入一样
    for(int i = 0; i < k; ++i){
        q->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = q;
    }
    return true;
}

//搜索指定的key
int SearchByKey(SkipList *sl, int key)
{
    SkipListNode *p, *q = NULL;
    int k = sl->level;
    p = sl->head;
    for(int i = k-1; i >= 0; i--){
        while((q = p->forward[i]) && (q->key <= key)){
            if(q->key == key)
                return (q->key);
            p = q;
        }
    }
    return 0;
}

//删除指定的key
bool deleteNode(SkipList *sl, int key)
{
    SkipListNode *p, *q = NULL;
    SkipListNode *update[LEVEL_MAX_LEN];
    p = sl->head;
    int k = sl->level;
    for(int i = k-1; i >= 0; i--){
        while((q = p->forward[i]) && (q->key < key)){
            p = q;
        }
        update[i] = p;
    }
    if(q && q->key == key){
        //逐层删除，和删除普通链表一样
        for(int i = 0; i < sl->level; ++i){
            if(update[i]->forward[i] == q){
                update[i]->forward[i] = q->forward[i];
            }
        } 
        free(q);
        //如果删除的是最大层的节点，那末需要重新维护跳表
        for(int i = sl->level-1; i >= 0; i--){
            if(sl->head->forward[i] == NULL){
                sl->level--;
            }
        }
        return true;
    }else{
        return false;
    }
}

//打印跳跃表
void PrintSkipList(SkipList *sl)
{
    SkipListNode *p, *q = NULL;
    //从最高层开始打印
    int  k = sl->level;
    for(int i = k-1; i >= 0; i--){
        p = sl->head;
        while(q = p->forward[i]){
            printf("%d->", p->data);
            p = q;
        }
        printf("\n");
    }
    printf("\n");
}

int main(void)
{
    SkipList *sl = CreatSkipList();
    for(int i = 0; i <= 19; ++i){
        InsertNode(sl, i, i*2);
    }
    PrintSkipList(sl);
    int m = SearchByKey(sl, 4);
    printf("serch value = %d\n", m);
    
    bool b = deleteNode(sl, 4);
    if(b){
       printf("删除成功\n");
    }
    PrintSkipList(sl);
    system("pause");
    return 0;
}

