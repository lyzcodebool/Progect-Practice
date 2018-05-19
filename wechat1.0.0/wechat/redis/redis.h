#pragma once
#ifndef _REDIS_H_
#define _REDIS_H_
#include<string.h>
#include<string>
#include<iostream>
#include<stdio.h>
#include<hiredis/hiredis.h>

class Redis{
public:
    Redis(){};
    ~Redis(){
        this->_connect = NULL;
        this->_reply = NULL;
    }
    
    redisContext* redis_connect(const char *ip, int port, const int timeofout);
    std::string getkey(std::string key);
    void setkey(std::string key, std::string value);
    void redisServerInfo(redisContext *_connect);
private:
    redisContext *_connect;
    redisReply *_reply;
};

redisContext* Redis::redis_connect(const char *ip, int port, const int timeofout)
{
    struct timeval timeout;
    timeout.tv_sec = timeofout;
    timeout.tv_usec = 0;
    this->_connect = redisConnectWithTimeout(ip, port, timeout);
    if(this->_connect != NULL && _connect->err != 0){
        std::cout<<"connect redis error: %s"<<_connect->errstr<<std::endl;
    }
    std::cout<<"connect redis successful!"<<std::endl;
    Redis::redisServerInfo(_connect);
    return this->_connect;
}

std::string Redis::getkey(std::string key)
{
    this->_reply = (redisReply *)redisCommand(this->_connect, "GET %s", key.c_str());
    std::string str = this->_reply->str;
    freeReplyObject(this->_reply);

    return str;
}

void Redis::setkey(std::string key, std::string value)
{
    this->_reply = (redisReply *)redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
    if(this->_reply == NULL){
        freeReplyObject(this->_reply);
    }
}

void Redis::redisServerInfo(redisContext *_connect)
{
    std::cout<<"this is the redis server informations:"<<std::endl;
    std::cout<<"fd:"<<_connect->fd<<std::endl;
    std::cout<<"tcp->host:"<<_connect->tcp.host<<std::endl;
    std::cout<<"tcp->port:"<<_connect->tcp.port<<std::endl;
    //std::cout<<"tcp->source addr:"<<_connect->tcp.source_addr<<std::endl;   
}

#endif
