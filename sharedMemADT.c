
#include "includes/sharedMemADT.h"

typedef struct sharedMemCDT {
    char shm_name[MAX_NAME_LENGTH];
    int fd;
    char *to_return;
    size_t max_size;
    char readable_name[MAX_NAME_LENGTH];
    sem_t *readable; 
    char close_name[MAX_NAME_LENGTH];
    sem_t *close_sem;
    int idx;
} *sharedMemADT;

sharedMemADT init_shared_memory(char * name, int mode) {
    sharedMemADT shm = malloc(SHM_SIZE);

    int max_size = MAX_SIZE;

    sprintf(shm->shm_name, "/%s", name);
    
    //create shared memory
    int fd = shm_open(shm->shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    
    check_error(fd, SHARED_MEMORY_OPEN_ERROR);

    check_error(ftruncate(fd, max_size), TRUNCATING_ERROR);

    shm->to_return = mmap(NULL, max_size, mode, MAP_SHARED, fd, 0);

    check_map_error(shm->to_return);

    snprintf(shm->readable_name, MAX_NAME_LENGTH, "/%s_%s", READABLE, name);
    shm->readable = sem_open(shm->readable_name, O_CREAT, S_IRUSR | S_IWUSR, 0); 
    check_error_sem(shm->readable);

    snprintf(shm->close_name, MAX_NAME_LENGTH, "/%s_%s", CLOSE_SEM, name);
    shm->close_sem = sem_open(shm->close_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
    check_error_sem(shm->close_sem);
    
    //save data in shared memory ADT 
    shm->fd = fd;
    shm->max_size = max_size;
    shm->idx = 0;
    return shm;
}

void check_error_sem(sem_t *sem) {
    if (sem == SEM_FAILED) {
        check_error(ERROR, SEMAPHORE_OPENING_ERROR);
    }
}

void check_map_error(void *ptr) {
    if (ptr == MAP_FAILED) {
        check_error(ERROR, MAPPING_ERROR);
    }
}

void write_to_shared_memory(sharedMemADT shm, const char * buff, int size) {
    int bytes_written = sprintf(&(shm->to_return[shm->idx]), "%s", buff);
    if (bytes_written > 0) {
        shm->idx += bytes_written;
    }
    check_error(sem_post(shm->readable), SEMAPHORE_POST_ERROR);  // Indicate data is ready to read
}


int read_from_shared_memory(sharedMemADT shm, char * buff) {
    check_error(sem_wait(shm->readable), SEMAPHORE_WAIT_ERROR);
    int bytes_read = sprintf(buff, "%s", &(shm->to_return[shm->idx]));
    shm->idx += bytes_read;
    if(shm->to_return[shm->idx-1] == '+'){
        // printf("De nada Sangui por encontrar el error\n");
        buff[bytes_read-1] = '\0';
        return -1;
    }
    return bytes_read;
}

void post_readable(sharedMemADT shm) {
    check_error(sem_post(shm->readable), SEMAPHORE_POST_ERROR);
   // write_to_shared_memory(shm, "\0", 1);
}

void wait_close(sharedMemADT shm) {
    check_error(sem_wait(shm->close_sem), SEMAPHORE_WAIT_ERROR);
}

void post_close(sharedMemADT shm) {
    check_error(sem_post(shm->close_sem), SEMAPHORE_POST_ERROR);
}

void close_shared_memory(sharedMemADT shm) {
    munmap(shm->to_return, shm->max_size);
    close(shm->fd);
    sem_close(shm->readable);
    sem_close(shm->close_sem);
    
}


void destroy_shared_memory(sharedMemADT shm) {
    check_error(sem_unlink(shm->readable_name), SEMAPHORE_DESTROY_ERROR);
    check_error(sem_unlink(shm->close_name), SEMAPHORE_DESTROY_ERROR);
    check_error(shm_unlink(shm->shm_name), SHARED_MEMORY_UNLINKING_ERROR);
    free(shm);
}


