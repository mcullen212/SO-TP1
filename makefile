# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Iincludes

# Executables and object files
EXECUTABLES = app view slave
OBJECTS = app.o view.o slave.o error.o

# Default target
all: $(EXECUTABLES)

# Compiling object files
app: app.c
	$(CC) $(CFLAGS) app.c error.c sharedMemADT.c -o app

view: view.c
	$(CC) $(CFLAGS) view.c error.c sharedMemADT.c -o view

slave: slave.c
	$(CC) $(CFLAGS) slave.c error.c -o slave

# Clean target
clean:
	rm -f $(EXECUTABLES) results.txt

