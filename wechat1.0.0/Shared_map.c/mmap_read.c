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
    fd = open(argv[1], O_RDWR);
    if(fd == -1){
        ERR_EXIT("open");
    }
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
    p =(STU*)mmap(NULL, sizeof(STU)*5, PROT_READ|PROT_WRITE, MAP_SHARED, fd ,0);
    if(p == NULL){
        ERR_EXIT("mmap");
    }
    int i;
    for(i = 0; i < 10; ++i){
        printf("name = %s age = %d\n", (p+i)->name, (p+i)->age);
    }
    printf("initialize over\n");//初始化结束
    /*取消映射*/
    munmap(p, sizeof(STU)*10);
    printf("exit...\n");

    return 0;
}
