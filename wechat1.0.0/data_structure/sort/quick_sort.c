/*************************************************************************
	> File Name: quick_sort.c
	> Author: 
	> Mail: 
	> Created Time: Wed 21 Mar 2018 12:19:38 AM PDT
 ************************************************************************/

#include<stdio.h>
#include"swap.h"

int a[] = {2, 4, 1, 5, 0};
void QuickSort(int l, int r)
{
    if(l >= r)
        return;
    int i = l, j = r;
    int x= a[0];
    while(i < j)
    {
        while(i < j && a[j] >= x)
            j --;
        if(i < j)
            a[i ++] = a[j];
        while(i < j && a[i] <= x)
            i++;
        if(i < j)
            a[j--] = a[i];
    }
    a[i] = x;
    QuickSort(l, i-1);
    QuickSort(i+1, r);
}

int main(void)
{
    QuickSort(0, 4);
    for(int i = 0; i < 5; ++i)
    {
        printf("%d ", a[i]);
    }
    putchar(10);
}

