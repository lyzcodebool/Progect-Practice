/*
 * g_net_update.c
 *  *
 *   *  Created on: 2015��2��5��
 *    *      Author: vae
 *     */
 /********************** g_net update version 2.0 ***************************/
 #include "config_info.h"
 #include "log.h"
 #include "epoll_connect.h"
 #include "thread_pool.h"
 #include "g_net_update.h"
 #include "database_process.h"


 #defineCONNECT_TO_SQL_SUCCESS0
 #define SERVER_TIMEOUT60 * 1 //60S

 static CONFIG_INFO config_info;
 static int epoll_fd = -1; // the epoll fd
 static int listen_fd = -1; // the socket fd socket create return
 static pthread_t accep_thread_t;
 static pthread_t send_thread_t;

 static int current_connected_total = 0; // the connections number
 static int exit_accept_flag = 0; // is exit the accept
 static int exit_flag = 0; // is exit the epoll wait
 static int port = 8111;

 static void closesocket(int fd);
 static void dumpInfo(unsigned char *info, int length);
 pthread_mutex_t connect_total_count_mutex = PTHREAD_MUTEX_INITIALIZER;
 static void connect_total_count_opration(BOOL add_or_subract, int value)
 {
        pthread_mutex_lock(&connect_total_count_mutex);
        if (add_or_subract)
     {
                current_connected_total = current_connected_total + value;
            
     }
        else
     {
                current_connected_total = current_connected_total - value;
            
     }
        pthread_mutex_unlock(&connect_total_count_mutex);

 }

 static void signal_handler_reboot(int32_t theSignal)
 {
        int i;
        int sockfd;
        char log_str_buf[LOG_STR_BUF_LEN];
        signal(SIGPIPE, SIG_IGN);
        if (SIGKILL == theSignal || SIGTERM == theSignal) //we can know when system excute child thread is end
     {
                snprintf(log_str_buf, LOG_STR_BUF_LEN, "receive kill signal exit the server.");
                LOG_INFO(LOG_LEVEL_FATAL, log_str_buf);
                if (listen_fd != -1)
         {
                        closesocket(listen_fd);
                        listen_fd = -1;
                    
         }
                exit_flag = 1;
                exit_accept_flag = 1;
                for (i = 0; i < MAX_EVENTS; i++)
         {
                        sockfd = get_fd_by_event_index(i);
                        if (sockfd != -1)
             {
                                closesocket(sockfd);
                            
             }
                    
         }
         #if CONNECT_TO_SQL_SUCCESS
            sql_pool_destroy();
         #endif
            
     }
        else if (SIGPIPE == theSignal)
     {
                LOG_INFO(LOG_LEVEL_INFO, "SIGPIPE received.\n");
            
     }

 }

 static void closesocket(int fd)
 {
        shutdown(fd, SHUT_RDWR);
        close(fd);

 }

 static int set_non_blocking(int sock)
 {
        int opts = -1;
        char log_str_buf[LOG_STR_BUF_LEN];

        opts = fcntl(sock, F_GETFL);
        if (opts < 0)
     {
                snprintf(log_str_buf, LOG_STR_BUF_LEN, "fcntl(sock=%d,GETFL).\n", sock);
                LOG_INFO(LOG_LEVEL_ERROR, log_str_buf);
                return (-1);
            
     }
        opts = opts | O_NONBLOCK;
        if (fcntl(sock, F_SETFL, opts) < 0)
     {
                snprintf(log_str_buf, LOG_STR_BUF_LEN, "fcntl(sock=%d,GETFL).\n", sock);
                LOG_INFO(LOG_LEVEL_ERROR, log_str_buf);
                return (-1);
            
     }
        return 1;

 }

 CONFIG_INFO *get_config_info(void)
 {
        return &config_info;

 }
 /****************************** accept task ******************************************/
 static void *accept_thread(void *arg)
 {
        int connect_fd = -1;
        socklen_t clilen = 0;
        struct sockaddr_in serveraddr;
        struct sockaddr_in clientaddr;
        struct sockaddr cliaddr;
        struct epoll_event ev;
        int val = 1;
        int err = 0;
        int bufsize = 32 * 1024; //1024*2048;
        int epoll_connect_event_index = -1;
        char log_str_buf[LOG_STR_BUF_LEN] = {0};

        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == listen_fd) // INVALID_SOCKET
     {
                LOG_INFO(LOG_LEVEL_ERROR, "create socket error.\n");
                return NULL;
            
     }
        /*set socket options*/
        err = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
        if (0 != err)
     {
                LOG_INFO(LOG_LEVEL_ERROR, "setsockopt SO_REUSEADDR error.\n");
                return NULL;
            
     }
        err = setsockopt(listen_fd, SOL_SOCKET, SO_RCVBUF, (char*)(&bufsize), sizeof(int));
        if (0 != err)
     {
                LOG_INFO(LOG_LEVEL_ERROR, "setsockopt SO_RCVBUF error.\n");
                return NULL;
            
     }

        val = 2; //2 minitues
        err = setsockopt(listen_fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &val, sizeof(val));
        if (0 != err)
     {
                LOG_INFO(LOG_LEVEL_ERROR, "setsockopt TCP_DEFER_ACCEPT error.\n");
                return NULL;
            
     }

     /*set_non_blocking(listen_fd);*/
        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_addr.s_addr = INADDR_ANY;
        serveraddr.sin_port = htons(/*config_info.port*/port); //serveraddr.sin_port  = htons(SERVER_LISTEN_PORT);
        serveraddr.sin_family = AF_INET;
        if (-1 == bind(listen_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) // SOCKET_ERROR
 {
            snprintf(log_str_buf, LOG_STR_BUF_LEN, "bind config port %d error.\n", /*config_info.port*/port);
            LOG_INFO(LOG_LEVEL_ERROR, log_str_buf);
            printf("bind config port %d error.\n", /*config_info.port*/port);
            return NULL;
        
 }
    printf("bind config port %d success.\n", /*config_info.port*/port);

        listen(listen_fd, LISTENQ); // 4096
            clilen = sizeof(cliaddr);
                while (!exit_accept_flag)
                {
                            if (current_connected_total < MAX_EVENTS) // effective
                    {
                                    if ((connect_fd = accept(listen_fd, (struct sockaddr *)&clientaddr, &clilen)) < 0)
                        {
                                            if ((errno != EAGAIN) && (errno != ECONNABORTED) && (errno != EPROTO) && (errno != EINTR))
                            {
                                                    snprintf(log_str_buf, LOG_STR_BUF_LEN, "accept error %d,%s.\n", errno, strerror(errno));
                                                    LOG_INFO(LOG_LEVEL_ERROR, log_str_buf);
                                                
                            }
                                            continue;
                                        
                        }
                                
                    }
                            else
                    {
                                    sleep(1);
                                    snprintf(log_str_buf, LOG_STR_BUF_LEN, "current accept number achieve to maximum(%d) .\n", MAX_EVENTS);
                                    LOG_INFO(LOG_LEVEL_INDISPENSABLE, log_str_buf);
                                    continue;
                                
                    }
                            epoll_connect_event_index = get_epoll_connect_free_event_in= "i have get you data";
                        int sockfd = parameter->fd;
                        printf("get buffer = %s\n", parameter->recv_buffer);
                        printf("sockfd = %d\n", sockfd);
                        send_buffer_to_fd(sockfd, send_buffer, strlen(send_buffer));
                        /* deal with you logic*/
                        if (sockfd != -1)
                    {
                                int mactched_event_index = get_matched_event_index_by_fd(sockfd);
                                connect_total_count_opration(FALSE, 1);
                                snprintf(log_str_buf, LOG_STR_BUF_LEN, "send data to stb over then close the socket(%d), *****client addr(%s)***** , thread_index = %d.\n", sockfd, get_client_addr_by_index(mactched_event_index), thread_index);
                                LOG_INFO(LOG_LEVEL_INDISPENSABLE, log_str_buf);
                                free_event_by_index(mactched_event_index);
                                closesocket(sockfd);
                        umber);
                                    LOG_INFO(LOG_LEVEL_INDISPENSABLE, log_str_buf);
                                
                    }
                            epoll_events_number = epoll_wait(epoll_fd, events, MAX_EVENTS, 2000); //2seconds
                            for (index = 0; index < epoll_events_number; ++index) // deal with the event
                    {
                                    connect_socket_fd_temp = events[index].data.fd; // get the socket fd
                                    /* delete epoll event*/
                                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[index].data.fd, &ev) == -1)
                        {
                                            snprintf(log_str_buf, LOG_STR_BUF_LEN, "EPOLL_CTL_DEL %d,%s.\n", errno, strerror(errno));
                                            LOG_INFO(LOG_LEVEL_ERROR, log_str_buf);
                                            events[index].data.fd = -1;
                                        
                        }
                                    if (events[index].events & EPOLLIN) //have read event
                        {
                                            int event_index = -1;
                                            int recv_length = 0;
                                            unsigned char recv_buffer[BUFFER_SIZE];

                                            if (connect_socket_fd_temp < 0)
                            {
                                                    connect_total_count_opration(FALSE, 1);
                                                    snprintf(log_str_buf, LOG_STR_BUF_LEN, "Event[%d] read invalid handle.\n", index);
                                                    LOG_INFO(LOG_LEVEL_ERROR, log_str_buf);
                                                    continue;
                                                
                            }
                                            event_index = get_matched_event_index_by_fd(connect_socket_fd_temp);
                                            snprintf(log_str_buf, LOG_STR_BUF_LEN, "Epoll get Event[%d] fd = %d.\n", event_index, connect_socket_fd_temp);
                                            LOG_INFO(LOG_LEVEL_INDISPENSABLE, log_str_buf);
                                            /*no the event*/
                                            if (event_index < 0)
                            {
                                                    connect_total_count_opration(FALSE, 1);
                                                    snprintf(log_str_buf, LOG_STR_BUF_LEN, "not find matched fd = %d.\n", connect_socket_fd_temp);
                                                    LOG_INFO(LOG_LEVEL_ERROR, log_str_buf);
                                                    free_event_by_index(event_index);
                                                    if (connect_socket_fd_temp != -1)
                                {
                                                            closesocket(connect_socket_fd_temp);
                                                            connect_socket_fd_temp = -1;
                                                        
                                }
                                                    continue;
                                                
                            }
                                            /* receive the buffer from the socket fd*/
                                            if (0 == recv_buffer_from_fd(connect_socket_fd_temp, recv_buffer, &recv_length))
                            {
                                                    snprintf(log_str_buf, LOG_STR_BUF_LEN, "recv_length = %d, current fd = %d, current job queue job number = %d.\n",recv_length, connect_socket_fd_temp, get_jobqueue_number(thpool));
                                                    LOG_INFO(LOG_LEVEL_INDISPENSABLE, log_str_buf);
                                                    dumpInfo((unsigned char *)recv_buffer, recv_length);
                                                    /*receive buffer success then add the thread pool*/
                                                    thpool_add_work(thpool, (void*)respons_stb_info, connect_socket_fd_temp, recv_buffer);
                                                
                            }
                                            else
                            {
                                                    /* receive buffer error*/
                                                    connect_total_count_opration(FALSE, 1);
                                                    snprintf(log_str_buf, LOG_STR_BUF_LEN, "Epoll event[%d] not read data, and the socket fd = %d.\n", event_index, connect_socket_fd_temp);
                                                    LOG_INFO(LOG_LEVEL_INDISPENSABLE, log_str_buf);
                                                    free_event_by_index(event_index);
                                                    if (connect_socket_fd_temp != -1)
                                {
                                                            closesocket(connect_socket_fd_temp);
                                                            connect_socket_fd_temp = -1;
                                                        
                                }
                                                
                            }
                            
                        }


                                    else
                                    {
                                                        snprintf(log_str_buf, LOG_STR_BUF_LEN, "Unknown error! event.data.fd = %d.\n", events[index].data.fd);
                                                        LOG_INFO(LOG_LEVEL_ERROR, log_str_buf);
                                                        connect_total_count_opration(FALSE, 1);
                                                        if (connect_socket_fd_temp < 0)
                                        {
                                                                LOG_INFO(LOG_LEVEL_ERROR, "EPOLLOUT.\n");
                                                                continue;
                                                            
                                        }
                                                        /*close the socket*/
                                                        free_event_by_index(get_matched_event_index_by_fd(connect_socket_fd_temp));
                                                        if (connect_socket_fd_temp != -1)
                                        {
                                                                closesocket(connect_socket_fd_temp);
                                                                connect_socket_fd_temp = -1;
                                                            
                                        }
                                                    
                                    }
                                            
                    }
                        
                }
                    log_close();
                        if (listen_fd != -1)
                        {
                                    closesocket(listen_fd);
                                    listen_fd = -1;
                                
                        }

                        #if CONNECT_TO_SQL_SUCCESS
                            sql_pool_destroy();
                            #endif
                                exit_accept_flag = 1;
                                    thpool_destroy(thpool);
                                        printf("[%s %s %d]Done...\n", __FILE__, __FUNCTION__, __LINE__);
                                            return 1;

 }



