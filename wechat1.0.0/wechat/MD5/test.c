#include <stdio.h>

int main(void)
{
    unsigned int a = 0; 
    a = a >> 3 & 0x3f;


    printf("%x", a);
}
