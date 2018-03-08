.PHONY:clean
cc=gcc
CFLACS=-Wall -g
BIN=mainftpd
OBJS=main.o ftpproto.o privparent.o sysutil.o session.o strtool.o tunable.o parseconf.o 
LIBS=-lcrypt
$(BIN):$(OBJS)
		$(CC) $(CFLACS) $^ -o $@ $(LIBS)
%.o:%.c
		$(CC) $(CFLACS) -c $< -o $@
clean:
		rm -f *.o $(BIN)

