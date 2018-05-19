#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include "redis.h"
#include "randcount.h"
#include <string>
#include <time.h>
#include <hiredis/read.h>

#define ONE_WEEK_IN_SECONDS 7*86400
#define VOTE_SCORE 432

#define ERR_EXIT(m)\
do{\
    perror(m);\
    exit(1);\
}while(0)

typedef struct user{
    std::string userid;
}userInfo;

int id = dist(rand_generator);
//在redis中建立
/* //建立投票功能需要的数据存储结构 */
void creatRedisObject(redisContext *_conn)
{
    std::string key;
    std::string value;
    std::string field;
    int userid = ++id;
    std::cout<<"userid = "<<userid<<std::endl;
    char *commond = (char *)malloc(sizeof(char)*256);
    if(commond == NULL)
        ERR_EXIT("malloc commond");
    //建立hset结构，用来存储文章信息
    std::cout<<"hset"<<std::endl;
    std::cin>>key;
    std::cin>>field;
    std::cin>>value;
    
    redisReply *reply_hash = (redisReply*)redisCommand(_conn, "hset %s %s %s",key.c_str(), field.c_str(), value.c_str());
    if(reply_hash == NULL)
        ERR_EXIT("redisReply ERROR");
    //建立set集合，用来存储一篇文章的已投票用户，保证唯一投票
    std::cout<<"set"<<std::endl;
    std::cin>>key;
    std::cin>>value;
    redisReply *reply_set = (redisReply*)redisCommand(_conn, "sadd %s %s", key.c_str(), value.c_str());
    if(reply_set == NULL)
        ERR_EXIT("redisReply ERROR");
    //建立zset集合,用来做排行榜，包括发布时间排行榜和票数排行榜
    std::cout<<"order set"<<std::endl;
    std::cin>>key;
    std::cin>>value;
    redisReply *reply_zset = (redisReply*)redisCommand(_conn, "zsadd %s %s %d", key.c_str(), value.c_str(), time(0));
    if(reply_zset == NULL)
        ERR_EXIT("redisReply ERROR");
}

//投票功能的实现
void article_vote(redisContext *_conn, std::string key, userInfo reader)
{
    char *commond = (char *)malloc(sizeof(char)*256);
    sprintf(commond, "smembers %s", key.c_str());
    redisReply *reply = (redisReply *)redisCommand(_conn, commond);
    if(reply == NULL)
        ERR_EXIT("redisReply_vote");
    int size = reply->elements;//获取当前投票的人数
    int i = 0;
    while(i < size)
    {
        redisReply r = reply->element[i++][0];
        if(!strcmp(reader.userid.c_str(), r.str)){//如果读者已经投票，就退出
            break;
        }else{
            if(i != size)
                continue;
            else{//该读者没有投票,开始投票，在当前文章集合中加入该用户id
                free(commond);
                commond = NULL;
                commond = (char *)malloc(sizeof(char)*256);
                sprintf(commond, "sadd %s %s", key.c_str(), reader.userid.c_str());
                redisReply *reply_01 = (redisReply *)redisCommand(_conn, commond);
                if(reply_01 == NULL)
                    ERR_EXIT("redisReply_vote_sadd");
                //这里文章的票数要在文章的hash结构中原子性+1;
                //zincrby增加文章积分和hincrby增加hash中的票数
            }
        }
    }
}

//获得文章获得的票数
void getVotes(redisContext *_conn, std::string key)
{
    char *commond = (char *)malloc(sizeof(char) * 256);
    sprintf(commond, "smembers %s", key.c_str());
    redisReply *reply = (redisReply *)redisCommand(_conn, commond);
    if(reply == NULL)
        ERR_EXIT("redisReply_vote");
    int size = reply->elements;
    std::cout<<"这篇文章获得了"<<size<<"票"<<std::endl;

    //可以直接从hash中直接获取票数
    /* char *command = (char*)malloc(sizeof(char)*256); */

    /* sprinf(command,"HGET %s %s",article,key); */
    /* redisReply *reply = redisCommand(conn,command); */
    /* if(reply->type==REDIS_REPLY_STRING) */
    /*     return reply->str; */
    /* else if(reply->type==REDIS_REPLY_INTEGER) //如果要获取的值是数值，那么直接转换为字符串 */
    /* { */
    /*     char *str = (char*)malloc(sizeof(char)*999999); */
    /*     sprintf(str,"%l",reply->integer); */
    /*     return str; */
    /* } */
    int i = 0;
    while(i <= size)
    {
        redisReply r = reply->element[i++][0];
        std::cout<<"以下用户赞了这篇文章:"<<std::endl;
        std::cout<<r.str<<std::endl;
    }
}

int main(void)
{
    Redis *redis = new Redis();
    redisContext* _conn = redis->redis_connect("127.0.0.1", 6379, 0);
    creatRedisObject(_conn);
    delete redis;
}

