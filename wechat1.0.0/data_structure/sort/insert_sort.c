/*************************************************************************
	> File Name: insert_sort.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Mar 2018 11:45:04 PM PDT
 ************************************************************************/

#include<stdio.h>
#include"swap.h"

int a[] = {2, 4, 1, 6, 3};
int main(void)
{
    int index, temp;
    for(int i = 1; i < 5; ++i)
    {
        index = i; temp = a[i];
        while(index > 0 && temp < a[index - 1]){
            swap(&a[index], &a[index - 1]);
            --index;
        }
    }

    for(int i = 0; i < 5; ++i){
        printf("%d ", a[i]);
    }
    putchar(10);
}
