#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

int main()
{
    FILE *fp = fopen("abc.txt", "rw+");
    if(fp == NULL){
        printf("open file error\n");
    }
    clock_t start = clock();
    for(int i = 0; i < 10000000; ++i){
       fgetc(fp);
       rewind(fp);
    }
    clock_t end = clock();
    double usetime = end - start;
    printf("usetime = %f\n", usetime/CLOCKS_PER_SEC);
    return 0;
}

