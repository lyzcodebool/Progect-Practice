.PHONY:clean
cc=gcc
CFLACS=-Wall -g
ALL=main
all:$(ALL)
OBJS=thread_pool.o main.o condition.o session.o sysutil.o dispatchertask.o 
.c.o:
		$(cc) $(CFLACS) -c $<
main:$(OBJS)
		$(cc) $(CFLACS) $^ -o $@ -lpthread -lrt
clean:
		rm -f $(ALL) *.o
		
