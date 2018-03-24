/*************************************************************************
	> File Name: swap.h
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Mar 2018 11:18:09 PM PDT
 ************************************************************************/

#ifndef _SWAP_H
#define _SWAP_H

void  swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
#endif
