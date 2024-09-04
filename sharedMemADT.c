#include "includes/sharedMemADT.h"

typedef struct sharedMemCDT {
    char shm_name[255];
    int fd;
    char *to_return;
    size_t max_size;
    size_t actual_size;
    char readable_name[255];
    sem_t *readable; 
    char writable_name[255];
    sem_t *writable;
} *sharedMemADT;

sharedMemADT init_shared_memory(pid_t pid, int amount_of_files) {
    sharedMemADT shm = malloc(SHM_SIZE);

    int max_size = MAX_SIZE(amount_of_files);

    char shm_name[255];
    sprintf(shm_name, "%s_%d", SHM_NAME, pid);

    //create shared memory
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    check_error(fd, SHARED_MEMORY_OPEN_ERROR);

    check_error(ftruncate(fd, max_size), TRUNCATING_ERROR);

    shm->to_return = mmap(NULL, max_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if(shm->to_return == MAP_FAILED) {
        check_error(ERROR, MAPPING_ERROR);
    }
    
    //create semaphore
    char readable_name[255];
    sprintf(readable_name, "%s_%d", SEM_NAME, pid);
    shm->readable = sem_open(readable_name, O_CREAT, O_RDWR, 0); 
    if(shm->readable == SEM_FAILED) {
        check_error(ERROR, SEMAPHORE_OPENING_ERROR);
    }
    

    char writable_name[255];
    sprintf(writable_name, "%s_%d", MUTEX_NAME, pid);
    shm->writable = sem_open(writable_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
    if(shm->writable == SEM_FAILED) {
        check_error(ERROR, SEMAPHORE_OPENING_ERROR);
    }
    
    //save data in shared memory ADT
    strcpy(shm->shm_name, shm_name); 
    shm->fd = fd;
    shm->max_size = max_size;
    shm->actual_size = 0;
    strcpy(shm->readable_name, readable_name);
    strcpy(shm->writable_name, writable_name);

    return shm;
}

void write_to_shared_memory(sharedMemADT shm, const char * buff) {
    sem_wait(shm->writable);

    size_t len = strlen(buff) + 1;  
    if (len > shm->max_size) {
        len = shm->max_size;
    }

    memcpy(shm->to_return, buff, len);
    shm->actual_size = len - 1;

    sem_post(shm->writable);
    sem_post(shm->readable);  
}

int read_from_shared_memory(sharedMemADT data, char * buff) {
    sem_wait(data->readable);  
    sem_wait(data->writable);  

    memcpy(buff, data->to_return, data->actual_size + 1);  
    int read_size = data->actual_size;

    sem_post(data->writable);

    return read_size;
}

void close_shared_memory(sharedMemADT shm) {
    
   
    sem_close(shm->writable);
    sem_unlink(shm->writable_name);  

    sem_close(shm->readable);
    sem_unlink(shm->readable_name);  

    close(shm->fd);
    shm_unlink(shm->shm_name);  

    munmap(shm->to_return, shm->max_size);   

    free(shm); 
}

