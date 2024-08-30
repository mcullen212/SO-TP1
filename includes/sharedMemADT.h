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

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE sizeof(struct sharedMemCDT)
#define SEM_NAME "/my_semaphore"


typedef struct sharedMemCDT * sharedMemADT;



// Function prototypes
struct shared_data* init_shared_memory(int is_creator);
void write_to_shared_memory(struct shared_data* data, const char* string);
void read_from_shared_memory(struct shared_data* data);
void cleanup_shared_memory(struct shared_data* data);

#endif // SHAREDMEMADT_H
