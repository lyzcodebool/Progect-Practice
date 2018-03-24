/*************************************************************************
	> File Name: head_sort.c
	> Author: 
	> Mail: 
	> Created Time: Thu 22 Mar 2018 08:00:59 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void HeadAdjust(int arr[], int i, int length)
{
    int child;
    int tmp;

    for(; 2*i+1 < length; i = child)
    {
        child = 2*i+1;
        if(child < length -1 && arr[child] < arr[child+1]){
            ++child;
        }
        if(arr[i] < arr[child]){
            swap(&arr[i], &arr[child]);
        }
        else 
            break;
    }
}

void HeadSort(int arr[], int length)
{
    int i;
    for(i = length/2 -1; i >= 0; i--){
        HeadAdjust(arr, i, length);
    }
    for(int j = length-1; j > 0; --j){
        swap(&arr[j], &arr[0]);
        HeadAdjust(arr, 0, j);
    }
}

void OutArray(int a[], int len)
{
    printf("len = %d\n", len);
    for(int i = 0; i < len; ++i)
    {
        printf("%d ", a[i]);
    }
    putchar(10);
}
int main(void)
{
    int num[7] = {98, 47, 88, 56, 123, 342, 999};
    int len = sizeof(num)/sizeof(int);
    printf("排序前：\n");
    OutArray(num, len);
    HeadSort(num, sizeof(num)/sizeof(int));
    printf("len = %ld\n", sizeof(num)/sizeof(int));
    printf("排序后:\n");
    OutArray(num, len);
}
