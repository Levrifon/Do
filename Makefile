CC = gcc
CFLAGS =  -Wall -Werror -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g


do: do.c makeargv.o 
	$(CC) $(CFLAGS) -o $@ $^

#makeargv.o :  makeargv.c makeargv.h
#	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	gcc  $(CFLAGS) -c $^
