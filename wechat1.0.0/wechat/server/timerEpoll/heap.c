#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int *buf;
    int bufLen;
    int heapSize;
}*heap_t;

void deleteHeapTop(heap_t heap)
{
    if(heap->heapSize == 0){
        printf("heap empty\n");
        exit(0);
    }
    int temp = heap->buf[0];
    int lastdata = heap->buf[heap->heapSize-1];
    heap->heapSize--;
    if(heap->heapSize == 0){
        printf("after delete empty\n");
        exit(1);
    }

    int i = 0;
    int j = (2 < 1)|1;
    while(j <= heap->heapSize-1){
        if(j < heap->heapSize && heap->buf[j] > heap->buf[j + 1])
            j ++;
        if(lastdata < heap->buf[j])
            break;
        heap->buf[i] = heap->buf[j];
        i = j;
        j = 2*i + 1;
    }
    heap->buf[i] = lastdata;
}
