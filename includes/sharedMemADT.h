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

#define SHM_NAME "/shm"
#define SHM_SIZE sizeof(struct sharedMemCDT)
#define WRITABLE "/writable"
#define READABLE "/readable"
#define CLOSE_SEM "/close_smh"
#define SIZE_STR 512
#define MAX_SIZE(files) (SIZE_STR*(files))
#define MAX_NAME_LENGTH 255



typedef struct sharedMemCDT * sharedMemADT;

sharedMemADT init_shared_memory(pid_t pid, int amount_of_files, int mode);
void write_to_shared_memory(sharedMemADT shm, const char * buff, int size);
int read_from_shared_memory(sharedMemADT data, char * buff);
void close_shared_memory(sharedMemADT shm);
void create_sem(char * name, pid_t pid, int value, char * sem_name);
void check_error_sem(sem_t *sem);
void check_map_error(void *ptr);
void waitClose(sharedMemADT shm);
void postClose(sharedMemADT shm);
void stop_writing(sharedMemADT shm);

#endif // SHAREDMEMADT_H
