/*************************************************************************
	> File Name: mmap_write.c
	> Author: 
	> Mail: 
	> Created Time: Sun 04 Feb 2018 02:10:06 AM PST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<error.h>
#include<fcntl.h>
#include<string.h>
#include <sys/mman.h>
/*这里要知道一个函数msync()
*这个函数是一个对映射的共享内存执行同步操作
*因为在网内存中写如数据的时候内核会找个合适的时机把数据写入文件
*如果想要即使写入文件可以调用者个函数
* */



/*
*映射不改变文件的大小
*映射内存可以大于文件的大小
*写入映射内存的数据写回文件的时候超过文件大小的部分不被写入。
*/
typedef struct stu{
    char name[4];
    int age;
}STU;

#define ERR_EXIT(m)\
        do\
        {\
            perror(m);\
            exit(EXIT_FAILURE);\
        }while(0)

int main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(stderr, "usage:%s <file>\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int fd;
    fd = open(argv[1], O_CREAT| O_RDWR|O_TRUNC, 0666);
    if(fd == -1){
        ERR_EXIT("open");
    }

    lseek(fd,sizeof(STU)*5-1, SEEK_SET);
    write(fd, "", 1);

    /*
    *mmap函数的使用
    *第一个参数是内存的随机或者指定的地址
    *第二个参数是引映射文件的大小，
    *第三个参数是保护模式
    *第四个参数是flag
    *第五个参数是文件指针（文件描述符）
    void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
    int munmap(void *addr, size_t length);
    * */
    STU *p;
    p =(STU*)mmap(NULL, sizeof(STU)*10, PROT_READ|PROT_WRITE, MAP_SHARED, fd ,0);
    if(p == NULL){
        ERR_EXIT("mmap");
    }
    char ch = 'a';
    int i;
    for(i = 0; i < 10; ++i){
        memcpy((p+i)->name, &ch, 1);
        (p+i)->age = 8*i;
        ch++;
    }
    printf("initialize over\n");//初始化结束
    /*取消映射*/
    sleep(10);
    munmap(p, sizeof(STU)*10);
    printf("exit...\n");

    return 0;
}
