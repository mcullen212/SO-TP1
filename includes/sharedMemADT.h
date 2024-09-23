#ifndef SHAREDMEMADT_H
#define SHAREDMEMADT_H

#include "error.h"
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_SIZE sizeof(struct sharedMemCDT)
#define READABLE "readable"
#define SIZE_STR (1024)
#define MAX_SIZE (SIZE_STR * SIZE_STR)
#define MAX_NAME_LENGTH (255)

typedef struct sharedMemCDT {
	char shm_name[MAX_NAME_LENGTH];
	int fd;
	char *to_return;
	size_t max_size;
	char readable_name[MAX_NAME_LENGTH];
	sem_t *readable;
	int idx;
} sharedMemADT;

sharedMemADT *init_shared_memory(char *name, int mode, sharedMemADT *shm);
void write_to_shared_memory(sharedMemADT *shm, const char *buff, int size);
int read_from_shared_memory(sharedMemADT *data, char *buff);
void close_shared_memory(sharedMemADT shm);
void create_sem(char *name, pid_t pid, int value, char *sem_name);
void destroy_shared_memory(sharedMemADT shm);
void post_readable(sharedMemADT shm);

#endif