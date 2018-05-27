#include <stdio.h>
#include "timer_heap.h"


int main(void)
{
    Heap_t *head = (Heap_t *)malloc(4096);
    int a = Insert_Heap(head, 1);
    int b = Insert_Heap(head, 2);
    int c = Insert_Heap(head, 3);
    int d = Insert_Heap(head, 4);


    deleteh(head);
    deleteh(head);
    deleteh(head);
    deleteh(head);
    for(int i = 0; i < head->size; ++i)
    {
        printf("%d ", head->buffer[i]);
    }
    putchar(10);
    return 0;
}
