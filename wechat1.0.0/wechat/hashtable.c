/*************************************************************************
	> File Name: hashtable.c
	> Author: 
	> Mail: 
	> Created Time: Thu 08 Mar 2018 01:32:11 AM PST
 ************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include"hashtable.h"
#include<string.h>

void init_kv(struct kv* kv)
{
    kv->next = NULL;
    kv->key = NULL;
    kv->value = NULL;
    kv->free_value = NULL;
}
void free_kv(struct kv* kv)
{
    if(kv){
        if(kv->free_value){
            kv->free_value(kv->free_value);
        }

        free(kv->key);
        kv->key = NULL;
        free(kv);
    }
}
static unsigned int hash_times33(char *key)
{
    unsigned int hash = 0;
    while(*key){
        hash = (hash << 5)+hash + *key ++;
    }
    return hash;
}

HashTable *hash_table_new()
{
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if(ht == NULL){
        hash_table_delete(ht);
        return NULL;
    }
    ht->table = (struct kv**)malloc(sizeof(struct kv *)*TABLE_SIZE);
    if(ht->table == NULL){
        hash_table_delete(ht);
        return NULL;
    }
    memset(ht->table, 0, sizeof(struct kv*)*TABLE_SIZE);
    return ht;
}

//删除哈希表所有值
void hash_table_delete(HashTable *ht)
{
    if(ht){
        if(ht->table){
            int i = 0;
            for(i; i < TABLE_SIZE; ++i){
                struct kv* p = ht->table[i];
                struct kv* q = NULL;
                while(p){
                    q = p->next;
                    free_kv(p);
                    p = q;
                }
            }
            free(ht->table);
            ht->table = NULL;
        }
        free(ht);
    }
}

//删除或者更新数据
int hash_table_put(HashTable *ht, char *key, void *value, void (*free_value)(void *))
{
    int i = hash_times33(key) % TABLE_SIZE;
    struct kv *p = ht->table[i];
    struct kv *tmp = p;
    while(p){
        if(strcmp(p->key, key) == 0){
            if(p->free_value){
                p->free_value(p->free_value);
            }
            p->value = value;
            p->free_value = free_value;
            break;
        }
        
        tmp = p;
        p = p->next;
    }
    if(p == NULL){
        char *kstr = malloc(sizeof(key) + 1);
        if(kstr == NULL){
            return -1;
        }
        struct kv *kv = (struct kv *)malloc(sizeof(kv));
        if(kv ==NULL){
            free(kstr);
            kstr = NULL;
            return -1;
        }
        
        init_kv(kv);
        kv->next = NULL;
        strcpy(kstr, key);
        kv->key = kstr;
        kv->value = value;
        kv->free_value = free_value;
        if(tmp == NULL){
            ht->table[i] = kv;
        }else{
            tmp->next = kv;
        }
    }
    return 0;
}

//根据关键字得到值索引
void *hash_table_get(HashTable *ht, char *key)
{
    int i = hash_times33(key)%TABLE_SIZE;
    struct kv *p = ht->table[i];
    while(p){
        if(strcmp(p->key, key) == 0){
            return p->value;
        }
        p = p->next;
    }

    return NULL;
}

//根据关键字删除一条消息
void hash_table_rm(HashTable *ht, char *key)
{
    int i = hash_times33(key)%TABLE_SIZE;
    struct kv *p = ht->table[i];
    struct kv *tmp = p;
    while(p){
        if(strcmp(p->key, key)){
            free_kv(p);
            if(tmp == p)
                ht->table[i] = NULL;
            else
                tmp->next = p->next;
        }
        tmp = p;
        p = p->next;
    }
}
