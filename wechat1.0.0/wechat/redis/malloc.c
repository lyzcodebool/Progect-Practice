#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <jemalloc/jemalloc.h>
#include <pthread.h>
//24
//40
//56
//72

void *func(void *arg)
{
    int size = *(int *)arg;
    char *p = (char *)malloc(size);
    if(p == NULL)
        printf("error");

    size_t n = malloc_usable_size(p);
    printf("%lu\n", n);
    free(p);
    return (void *)0;
}

int main(void)
{
    int *p = (int *)malloc(4097);
    size_t n = malloc_usable_size(p);
    printf("%lu\n", n);
    pthread_t tid;
    int size = 216;
    int ret = pthread_create(&tid, NULL, func, (void *)&size);
    if(ret == -1)
        printf("error pthread\n");
    free(p);

    pthread_join(tid, NULL);
}
