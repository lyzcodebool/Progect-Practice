/*************************************************************************
	> File Name: mubble_sort.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Mar 2018 11:17:17 PM PDT
 ************************************************************************/

#include<stdio.h>
#include"swap.h"

#define len sizeof(a)/sizeof(int)

int a[] = {2, 0, 1, 4, 3};

int main(void)
{
    for(int i = 0; i < len-1; ++i){
        for(int j = i+1; j < len; ++j){
            if(a[i] > a[j]){
                swap(&a[i], &a[j]);
            }
        }
    }

    for(int i = 0; i < len; ++i)
    {
        printf("%d ", a[i]);
    }
}


