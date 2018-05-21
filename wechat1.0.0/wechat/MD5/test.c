#include <stdio.h>

int main(void)
{
    int a = 64; 
    a = a >> 3 & 0x3f;


    printf("%x", a);
}
