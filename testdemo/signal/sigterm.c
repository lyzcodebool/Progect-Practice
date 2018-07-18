#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

void signal_handle(int signo)
{
    printf("close\n");
}

int main()
{
    FILE *fp = fopen("test.tex", "rw+");
    int n = 10;
    signal(SIGTERM, signal_handle);
    while(n){
    n--;
    fclose(fp); 
    }
    return 0;
}

