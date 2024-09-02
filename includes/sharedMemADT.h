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
#define SIZE_STR 512
#define MAX_SIZE(files) (SIZE_STR*(files))


typedef struct sharedMemCDT * sharedMemADT;

sharedMemADT init_shared_memory(pid_t pid, int is_creator, int amount_of_files);
void write_to_shared_memory(sharedMemADT data, const char* string);
int read_from_shared_memory(sharedMemADT data, char * buff);
void cleanup_shared_memory(sharedMemADT data);
void close_shared_memory(sharedMemADT shm);
void printName(sharedMemADT data);

#endif // SHAREDMEMADT_H
