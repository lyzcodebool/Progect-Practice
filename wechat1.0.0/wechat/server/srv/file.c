#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>



int main()
{
   int fd = open("text", O_CREAT| O_RDWR| O_NONBLOCK);
   if(fd == -1){
       printf("error\n");
        exit(1);
   }
   char buf[1024];
   while(1)
   {
    int ret = read(fd, buf, sizeof(buf));
    if(ret == -1){
       printf("read error\n");
    }

   }

    return 0;
}

