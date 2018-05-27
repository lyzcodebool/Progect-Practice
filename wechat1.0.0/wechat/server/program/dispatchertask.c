#include"../bin/dispatchertask.h"
#include"../bin/common.h"
#include"../bin/sysutil.h"

//epoll检测到事件处理函数
/*处理薪连接的回调函数*/
void *new_connect_task(void *arg)
{
    int *fd = (int*)arg;
    struct sockaddr_in client_addr;

    int new_conn;
    if((new_conn = accept_timeout(*fd, &client_addr, 5)) < 0){
        ERR_EXIT("accept_timeout");
    } 
    printf("conn == %d\n", new_conn);
    deactivate_nonblock(*fd);
    printf("connect->port: %hu\t  ->ip: %s\n", ntohs(client_addr.sin_port), inet_ntoa(client_addr.sin_addr)); 

    /*在epoll中注册新的socket描述符*/
    add_epoll_fd(epollfd, new_conn, 1);
    client_info[new_conn].client_fd = new_conn;
    return NULL;
}

/*写事件到来的回调函数*/
void *io_read_task(void *arg)
{
    printf("--------------\n");
    int *sockfd = (int *)arg;
    printf("sockfd = %d\n", *sockfd);
    activate_nonblock(*sockfd);
    char read_buff[BUFF_SIZE];
    int nread = 0;
    /*
     *ET模式下的读写问题
     *1.对于读，只要buff中有数据就一直读
     *2.对于写，只要buff还有空间而且用户写的请求还未完成，就一直写
     *ET模式针对上面来两种情况调用EPOLL_CTL_MOD返回读或者写就绪
     */
    if((nread = read(*sockfd, read_buff, BUFF_SIZE)) > 0)
    {
        read_buff[nread] = '\0';
        printf("%s\n", read_buff);

        if(nread == BUFF_SIZE && errno != EAGAIN){
            //数据还没读完，重新MOD IN事件
            read_ctl_mod(epollfd, *sockfd, TRUE);
        }
        else if(nread == BUFF_SIZE && errno == EAGAIN){
            close(*sockfd);
        }
        else
        {
            //buffer中的数据已经读取完毕MOD OUT事件
            read_ctl_mod(epollfd, *sockfd, TRUE);
        }
    }
    else if(nread == 0){
        close(*sockfd);
    }
    /*while((nread = read(*sockfd, read_buff + n, BUFF_SIZE-1)) > 0){
        n+=nread;
    }
    if(nread == -1 && errno != EAGAIN){
        ERR_EXIT("read");
    }
    read_ctl_mod(epollfd, *sockfd, TRUE);
    */
    memset(read_buff, 0, sizeof(read_buff));
    return NULL;
}
void *io_write_task(void *arg)
{
    /*暂不实现...*/
    return NULL;
}



