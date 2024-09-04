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

#define SHM_NAME "/shared_memory"
#define SHM_SIZE sizeof(struct sharedMemCDT)
#define SEM_NAME "/my_semaphore"
#define MUTEX_NAME "/my_mutex"
#define SIZE_STR 512
#define MAX_SIZE(files) (SIZE_STR*(files))


typedef struct sharedMemCDT * sharedMemADT;

sharedMemADT init_shared_memory(pid_t pid, int amount_of_files);
void write_to_shared_memory(sharedMemADT shm, const char * buff);
int read_from_shared_memory(sharedMemADT data, char * buff);
void close_shared_memory(sharedMemADT shm);

#endif // SHAREDMEMADT_H
