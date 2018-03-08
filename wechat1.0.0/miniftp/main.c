/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Tue 27 Feb 2018 01:46:16 AM PST
 ************************************************************************/

#include"common.h"
#include"sysutil.h"
#include"session.h"
#include"tunable.h"
#include"ftpproto.h"
#include"parseconf.h"

int main(void)
{
    if(getuid() != 0){
        fprintf(stderr, "miniftp: must be started as root");
        exit(EXIT_FAILURE);
    }
    //list_common();

    parseconf_load_file(MINIFTP_CONF);
    printf("tunable_pasv_enable = %d\n", tunable_pasv_enable);
    printf("tunable_port_enable = %d\n", tunable_port_enable);
    printf("tunable_listen_port = %u\n", tunable_listen_port);
    printf("tunable_max_clients = %u\n", tunable_max_clients);
    printf("tunable_max_per_ip = %u\n", tunable_max_per_ip);
    printf("tunable_accept_timeout = %u\n", tunable_accept_timeout);
    printf("tunable_connect_timeout = %u\n", tunable_connect_timeout);
    printf("tunable_idle_session_timeout = %u\n", tunable_idle_session_timeout);
    printf("tunable_data_connection_timeout = %u\n", tunable_data_connection_timeout);
    printf("tunable_local_umask = %u\n", tunable_local_umask);
    printf("tunable_upload_max_rate = %u\n", tunable_upload_max_rate);
    printf("tunable_download_max_rate = %u\n", tunable_download_max_rate);

    if(tunable_listen_address == NULL){
        printf("listen_address = NULL\n");
    }else{
        printf("tunable_listen_address = %s\n", tunable_listen_address);
    }
    
    session_t sess = {
        //控制连接
       0, -1, "....", "....", "....",
        //父子进程通道
        -1, -1,
        //FTP协议状态
        0,
        //数据连接
        NULL, -1
    };
    int listenfd = tcp_server(tunable_listen_address, tunable_listen_port);
    int conn;
    pid_t pid;

    while(1)
    {
        conn = accept_timeout(listenfd, NULL, 0);
        if(conn == -1)
            ERR_EXIT("accept_timeout");

        pid = fork();
        if(pid == 0){
            close(listenfd);
            sess.ctrl_fd = conn;
            begin_session(&sess);

        }else{
            close(conn);
        }
    }
    return 0;
}
