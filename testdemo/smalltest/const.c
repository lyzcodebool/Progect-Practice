#include <stdio.h>

int main()
{
    const int a = 100;
    //int *p = &a;
    int *p = (int *)&a;
    *p = 200;
    printf("a = %d\n *p = %d\n", a, *p);
    return 0;
}

