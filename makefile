CC = gcc
CFLAGS = -Wall -g -Iincludes

all: md5 view slave

md5: md5.o error.o
	$(CC) $(CFLAGS) -o md5 md5.o error.o

view: view.o error.o
	$(CC) $(CFLAGS) -o view view.o error.o

slave: slave.o error.o
	$(CC) $(CFLAGS) -o slave slave.o error.o

md5.o: md5.c includes/md5.h 
	$(CC) $(CFLAGS) -c md5.c

view.o: view.c includes/view.h 
	$(CC) $(CFLAGS) -c view.c

slave.o: slave.c 
	$(CC) $(CFLAGS) -c slave.c

error.o: error.c includes/error.h
	$(CC) $(CFLAGS) -c error.c

clean:
	rm -f *.o md5 view slave
