#include <stdio.h>
#include <pthread.h>

/* cup占用率高的定位分析`` */

void *worker1(void *arg)
{
    arg  = NULL;
    int a = 1;
    int b = 2;
    int sum  = 0;
    while(1){
        sum = a+b;
    }
    return (void *)0;
}

void *worker2(void *arg)
{
    arg = NULL;
    int a = 1;
    int b = 2;
    int sum = 0;
    sum = a + b;
    printf("sum = %d\n", sum);
    return (void *)0;
}


int main()
{
    pthread_t tid[10];
    for(int i = 0; i < 10; ++i){
        if(i <= 4)
            pthread_create(&tid[i], NULL, worker2, (void*)NULL);
        else if(i == 5)
            pthread_create(&tid[i], NULL, worker1, (void*)NULL);
        else
            pthread_create(&tid[i], NULL, worker2, (void*)NULL);
    }

    for(int i = 0; i < 10; ++i){
        pthread_join(tid[i], NULL);
    }
    return 0;
}

