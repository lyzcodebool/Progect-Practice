#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#define PREFIX_SIZE (sizeof(size_t))
void *zmalloc(size_t size)
{
    void *ptr = malloc(size + PREFIX_SIZE);
    if(ptr == NULL){
        //oom错误处理函数
        printf("error");
    }
    *((size_t *)ptr) = size;
    printf("%lu\n", *((size_t *)ptr));
    return ptr;
}


int main(void)
{
   int *p = (int *)zmalloc(1024);
   printf("%p\n", p);
   printf("zmalloc size->%lu\n", malloc_usable_size(p));
   
   int *p_c = (int *)malloc(0);
   printf("%p\n", p_c);
   printf("malloc size->%lu\n", malloc_usable_size(p_c));
   free(p_c);
   free(p);
}
