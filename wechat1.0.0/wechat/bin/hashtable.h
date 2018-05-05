/*************************************************************************
	> File Name: hashtable.h
	> Author: 
	> Mail: 
	> Created Time: Thu 08 Mar 2018 01:06:57 AM PST
 ************************************************************************/

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

//链地址法
#define TABLE_SIZE 1024*1024
typedef struct HashTable HashTable;
struct kv{
    struct kv *next;
    char *key;
    void *value;
    void(*free_value)(void *);
};

struct HashTable{
    struct kv **table;
};
static void init_kv(struct kv* kv);
static void free_kv(struct kv* kv);
static unsigned int hash_times33(char *key);

HashTable *hash_table_new();

//删除哈希表所有值
void hash_table_delete(HashTable *ht);

//删除或者更新数据
int hash_table_put(HashTable *ht, char *key, void *value, void (*free_value)(void *));

//根据关键字得到值索引
void *hash_table_get(HashTable *ht, char *key);

//根据关键字删除一条消息
void hash_table_rm(HashTable *ht, char *key);

#endif
