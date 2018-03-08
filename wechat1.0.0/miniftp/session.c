/*************************************************************************
	> File Name: session.c
	> Author: 
	> Mail: 
	> Created Time: Tue 27 Feb 2018 02:46:17 AM PST
 ************************************************************************/

#include"session.h"
#include"privparent.h"
#include"ftpproto.h"
#include"common.h"
void begin_session(session_t  *sess)
{
    int sockfds[2];
    /*socketpair用于创建两个未命名的，相互连接的unix域套接字*/
    if(socketpair(PF_UNIX, SOCK_STREAM, 0, sockfds) < 0)
        ERR_EXIT("socketpair");

    pid_t pid;
    pid = fork();
    if(pid < 0)
        ERR_EXIT("fork");
    if(pid == 0)
    {
        //ftp服务进程
        close(sockfds[0]);
        sess->child_fd = sockfds[1];
        handle_child(sess);
    }
    else
    {
        struct passwd *pw = getpwnam("nobody"); 
        if(pw == NULL)
            return;
    
        //将当前进程的有效组id改成pw_gid
        if(setegid(pw->pw_gid) < 0)
            ERR_EXIT("setegid");
        //将当前进程的有效用户id改成pw_uid
        if(seteuid(pw->pw_uid) < 0)
            ERR_EXIT("seteuid");

    
        //nobody进程
        close(sockfds[1]);
        sess->parent_fd = sockfds[0];
        handle_parent(sess);
    }
}
