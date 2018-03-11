#ifndef _DISPATCHER_TASK_H_
#define _DISPATCHER_TASK_H_

//epoll检测到事件处理回调函数
void *new_connect_task(void *arg);
void *io_read_task(void *arg);
void *io_write_task(void *arg);

#endif
