/*************************************************************************
	> File Name: select_sort.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Mar 2018 11:34:46 PM PDT
 ************************************************************************/

#include<stdio.h>
#include"swap.h"

int a[5] = {2 ,3, 1, 5 ,2};

int main(void)
{
    int k = 0;
    for(int i = 0; i < 5; ++i){
        k = i;
        for(int j = i+1; j < 5; ++j){
            if(a[k] > a[j]){
                k = j;
            }
            if(k != i){
                swap(&a[k], &a[i]);
            }
        }
    }
    for(int i = 0; i < 5; ++i)
    {
        printf("%d ", a[i]);
    }
    putchar(10);

    return 0;
}
