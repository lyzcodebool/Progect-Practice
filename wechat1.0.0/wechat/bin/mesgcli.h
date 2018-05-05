#ifndef  _MESG_SRV_H_
#define  _MESG_SRV_H_

typedef struct mesgcli{
    int sign_in;         //登录请求
    int registered;      //注册请求
    int chatroom;         //群聊请求
    int chatsigle;       //单聊请求
    int redbag_pub;      //红包发布
}MESGCLI;
#endif
