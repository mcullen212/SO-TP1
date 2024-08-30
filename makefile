CC = gcc
CFLAGS = -Wall -g -Iincludes

all: app view slave

md5: app.o error.o
	$(CC) $(CFLAGS) -o app app.o

view: view.o error.o
	$(CC) $(CFLAGS) -o view view.o error.o

slave: slave.o error.o
	$(CC) $(CFLAGS) -o slave slave.o error.o

md5.o: app.c includes/app.h 
	$(CC) $(CFLAGS) -c app.c error.c

view.o: view.c includes/view.h 
	$(CC) $(CFLAGS) -c view.c

slave.o: slave.c 
	$(CC) $(CFLAGS) -c slave.c error.c

error.o: error.c includes/error.h
	$(CC) $(CFLAGS) -c error.c

clean:
	rm -f *.o app view slave
