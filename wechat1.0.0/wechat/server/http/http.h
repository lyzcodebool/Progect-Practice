#ifndef _HTTP_H_
#define _HTTP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <error.h>
#include <assert.h>

#define BUFFER_SIZE 4096
#define LISTENQ 5
#define ERR_EXIT(m)\
    do{\
        perror("m"); \
        exit(1);  \
    }while(0)

struct sockaddr_in address;
/*http协议格式*/
/* GET/POST 用户指定的url(一般不带域名) HTTP协议版本 */
/* 字段1名: 字段内容\r\n */
/* 字段2名: 字段内容\r\n */
/* 字段3名: 字段内容\r\n */
/*     ... */
/*     ... */
/* 字段n名: 字段内容\r\n */
/* \r\n */
/* HTTP协议包体内容 */

//主状态机的两种可能状态
typedef  enum CHECK_STATE{
    CHECK_STATE_REQUESTLINE = 0, //当前正在分析请求行
    CHECK_STATE_HEADER           //当前正在分析头部信息
}CHECK_STATE;
CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;

//从状态机的三种可能状态，即行的读取状态
typedef  enum LINE_STATES{
    LINE_OK = 0,                 //读取到一个完整的行
    LINE_BAD,                    //行出错
    LINE_OPEN                    //行数据尚不完整
}LINE_STATES;

//服务器处理http请求的结果
typedef  enum HTTP_CODE{
    NO_REQUEST,                  //请求的数据不完整，需要继续读取数据
    GET_REQUEST,                 //得到一个完整的客户请求
    BAD_REQUEST,                 //表示请求有语法错误
    FORBIDDEN_REQUEST,           //表示客户堆请求的资源没有访问权限
    INTERNAL_REQUEST,            //表示服务器内部错误
    CLOSE_CONNECTION             //客户端关闭连接
}HTTP_CODE;

/*这里简化问题，不给客户端发送完整的应答报文，只是告知服务器处理请求的结果*/
static const char *szret[] = {"I go a correct result...\n", "something wrong...\n"};

/*
 * 从状态机，用于解析一行内容
 * chheck_index指向当前buffer当前正在分析的字节
 * read_index 指向buffer最有一个字节的下一个位置
 */
LINE_STATES parse_line(char *buf, int check_index,  int read_index)
{
    char temp;
    for(; check_index < read_index; ++check_index){
        //获取当前的分析字节
        temp = buf[check_index];
        if(temp == '\r'){
            //这里表示收到一个回车符，可能会收到一个完整的行
            if(check_index + 1 == read_index){
                //这里表示‘\r’就是这次读取的最后一个字符，所以数据没有读取完毕，需要继续读取
                return LINE_OPEN;
            }else if(buf[check_index + 1] == '\n') {
                //表示本次读取到一个完整的行
                buf[check_index ++] = '\0';
                buf[check_index ++] = '\0';
                printf("prase line_ok....\n");
                return LINE_OK;
            }else{
                //说明出现语法错误
                return LINE_BAD;
            }
        }
        else if(temp == '\n'){
            //如果遇到'\n'可能读到完整的行，需要检查前一个字节是不是回车符
            if(check_index > 1 && buf[check_index - 1] == '\r')
                return LINE_OK;
            return LINE_BAD;
        }
    }
    //本次解析没有遇到'\r'或者'\n',表示数据还没有读取完毕，需要继续读取
    return LINE_OPEN;
}

//分析请求行
HTTP_CODE parse_requestline(char *temp)
{
    printf("start parse requestline...\n");
    char *url = strpbrk(temp, " \t"); //检查temp中有没有"\t"
    //如果请求行中没有空白字符和"\t"字符，则http请求必有问题
    if(url == NULL){
        printf("url error...\n");
        return BAD_REQUEST;
    }
    *url++ = '\0';
    char *method = temp;
    printf("method = %s....\n", method);
    if(strcasecmp(method, "GET") == 0) //仅支持GET方法,比较字符串是否相同，忽略大小写。
    {
        printf("The request method is GET\n");
    }else{
        printf("parse requestline error...\n");
        return BAD_REQUEST;
    }

    url += strspn(url, " \t");  //找到url第一个不是\t的位置，作用是跳过http请求行的第一个空格或者\t，解析http协议版本
    printf("url = %s\n", url);
    char *version = strpbrk(url, " \t");
    if(!version){
        printf("version parse error\n");
        return BAD_REQUEST;
    }
    *version ++ = '\0';
    version += strspn(version, " \t");
    printf("version = %s\n", version);
    /*仅支持HTTP/1.1*/
    if(strcasecmp(version, "HTTP/1.1") != 0){
        printf("version not sample...\n");
        return BAD_REQUEST;
    }
    printf("url = %s\n", url);
    /*检查url是否合法*/
    if(strcasecmp(url, "http://baidu.com") == 0){
        url += 6;
        printf("                          url = %s\n", url);
        url = strchr(url, '/');
        printf("                          url = %s\n", url);
    }
    if(!url || url[0] != '/')
        return BAD_REQUEST;
    printf("The request the url is: %s\n", url);
    /*http请求行分析完毕，状态转移到头部字段的分析*/
    checkstate = CHECK_STATE_HEADER;
    printf("checkstate = %d\n", checkstate);
    return NO_REQUEST;
}

HTTP_CODE parse_headers(char *temp)
{
    /*遇到一个空行，说明我们得到一个正确的http请求*/
    printf("parse headers ...\n temp = %s\n", temp);
    if(temp[0] == '\0'){
        return GET_REQUEST;
    }else if(strncasecmp(temp, "Host:", 5) == 0){
        temp += 5;
        temp += strspn(temp, "\t");
        printf("The request host is %s\n", temp);
    }else{
        //其他头部字段都不处理
        printf("I can't handle the request\n");
    }

    return NO_REQUEST;
}

//分析http请求的入口函数
HTTP_CODE parse_content(char *buffer, int check_index, int read_index, int start_line)
{
    LINE_STATES linestatus = LINE_OK; //记录当前行的读取状态
    HTTP_CODE retcode = NO_REQUEST;    //记录http请求的处理结果
    /*主状态机， 用于从buffer中取出所有的完整的行*/
    while((linestatus = parse_line(buffer, check_index, read_index)) == LINE_OK){
        char *temp = buffer + start_line;   //start_line是行在buffer中的起始位置
        printf("temp = %s...\n", temp);
        start_line = check_index;

        /*checkstate 记录主状态机当前的状态*/
        switch(checkstate)
        {
            case CHECK_STATE_REQUESTLINE:               //第一个状态，分析请求行
                {
                    printf("check state requestline\n");
                    retcode = parse_requestline(temp);
                    printf("checkstate = %d\n", checkstate);
                    if(retcode == BAD_REQUEST)
                        return  BAD_REQUEST;
                    break;
                }
            case CHECK_STATE_HEADER:                     //第二个状态，分析头部字段
                {
                    printf("check state headers\n");
                    retcode = parse_headers(temp);
                    if(retcode == BAD_REQUEST)
                        return BAD_REQUEST;
                    else if(retcode == GET_REQUEST)
                        return GET_REQUEST;
                    break;
                }
            default:
                return INTERNAL_REQUEST;
        }
    }
    //若没有读取到一个完整的行, 则需要继续读取数据
    if(linestatus == LINE_OPEN)
        return NO_REQUEST;
    else
        return BAD_REQUEST;
}

/*获得一个监听socket描述符*/
int getsockfd(char **argv1, char **argv2)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        ERR_EXIT("socket");
    const char *ip = *argv1;
    const int port = atoi(*argv2);

    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int ret = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    if(ret < 0)
        ERR_EXIT("bind");
    if(listen(sockfd, LISTENQ) < 0)
        ERR_EXIT("listen");
    
    return sockfd;
}
#endif  /*_HTTP_H_*/
