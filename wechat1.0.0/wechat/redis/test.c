#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
int main(void)
{
 
  char *p,*q;
  void * brk;
 brk=sbrk(0);
printf("brk is ....%p...\n",brk);
  p = (char *)malloc(200);
 if(p==NULL)
  return 1;
 
 strcpy(p,"hello");
 
  brk=sbrk(100);
printf("111....brk is ....%p...\n",brk);
 printf("200,p is %s...\n",p);
 
  q= (char *)malloc(16);
  if(q==NULL)
   return 1;
 
 strcpy(q,"nihao");
 
 printf("16,q is %s...\n",q);
 
 free(q);
 free(p);
 
  p= (char *)malloc(16);
 
 return 0;
 
}
