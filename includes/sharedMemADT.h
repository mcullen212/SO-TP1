#ifndef SHAREDMEMADT_H
#define SHAREDMEMADT_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include <string.h>

#define SHM_SIZE sizeof(struct sharedMemCDT)
#define READABLE "readable"
#define SIZE_STR (1024)
#define MAX_SIZE (SIZE_STR*SIZE_STR)
#define MAX_NAME_LENGTH (255)



typedef struct sharedMemCDT * sharedMemADT;

sharedMemADT init_shared_memory(char * name, int mode);
void write_to_shared_memory(sharedMemADT shm, const char * buff, int size);
int read_from_shared_memory(sharedMemADT data, char * buff);
void close_shared_memory(sharedMemADT shm);
void create_sem(char * name, pid_t pid, int value, char * sem_name);
void destroy_shared_memory(sharedMemADT shm);
void post_readable(sharedMemADT shm);

#endif