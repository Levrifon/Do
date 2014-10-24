CC = gcc
CFLAGS =  -Wall -Werror -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g

do: do.c
	$(CC) $(CFLAGS) -o $@ $<
