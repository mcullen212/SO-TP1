#include "includes/sharedMemADT.h"

typedef struct sharedMemCDT {
    char shm_name[MAX_NAME_LENGTH];
    int fd;
    char *to_return;
    size_t max_size;
    char readable_name[MAX_NAME_LENGTH];
    sem_t *readable; 
    char writable_name[MAX_NAME_LENGTH];
    sem_t *writable;
    char close_name[MAX_NAME_LENGTH];
    sem_t *close_smh;
    
} *sharedMemADT;

sharedMemADT init_shared_memory(pid_t pid, int amount_of_files, int mode) {
    sharedMemADT shm = malloc(SHM_SIZE);

    int max_size = MAX_SIZE(amount_of_files);

    sprintf(shm->shm_name, "%s_%d", SHM_NAME, pid);

    //create shared memory
    int fd = shm_open(shm->shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    check_error(fd, SHARED_MEMORY_OPEN_ERROR);

    check_error(ftruncate(fd, max_size), TRUNCATING_ERROR);

    shm->to_return = mmap(NULL, max_size, mode, MAP_SHARED, fd, 0);

    check_map_error(shm->to_return);


    snprintf(shm->readable_name,MAX_NAME_LENGTH, "%s_%d", READABLE, pid);
    shm->readable = sem_open(shm->readable_name, O_CREAT, S_IRUSR | S_IWUSR, 0); 
    check_error_sem(shm->readable);

    snprintf(shm->writable_name, MAX_NAME_LENGTH, "%s_%d", WRITABLE, pid);
    shm->writable = sem_open(shm->writable_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
    check_error_sem(shm->writable);

    snprintf(shm->close_name, MAX_NAME_LENGTH, "%s_%d", CLOSE_SEM, pid);
    shm->close_smh = sem_open(shm->close_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
    check_error_sem(shm->close_smh);

    // printf("Semaphore writable name: %s\n", shm->writable_name);
    // printf("Semaphore readable name: %s\n", shm->readable_name);
    // printf("Semaphore close name: %s\n", shm->close_name);
    // printf("Shared memory name: %s\n", shm->shm_name);
    
    //save data in shared memory ADT 
    shm->fd = fd;
    shm->max_size = max_size;
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
    check_error(sem_wait(shm->writable), SEMAPHORE_WAIT_ERROR);

    snprintf(shm->to_return, size, "%s",buff);

    check_error(sem_post(shm->readable), SEMAPHORE_POST_ERROR);  // Indicate data is ready to read
    //check_error(sem_post(shm->writable), SEMAPHORE_POST_ERROR);  // Release writable semaphore
    // printf("Semaphore writable name: %s\n", shm->writable_name);
    // printf("Semaphore readable name: %s\n", shm->readable_name);
    // printf("Semaphore close name: %s\n", shm->close_name);
    // printf("Shared memory name: %s\n", shm->shm_name);
}

int read_from_shared_memory(sharedMemADT shm, char * buff) {
    check_error(sem_wait(shm->readable), SEMAPHORE_WAIT_ERROR);
    //check_error(sem_wait(shm->writable), SEMAPHORE_WAIT_ERROR);

    int read_size = sprintf(buff, "%s",shm->to_return);

    check_error(sem_post(shm->writable), SEMAPHORE_POST_ERROR);

    // printf("Semaphore writable name: %s\n", shm->writable_name);
    // printf("Semaphore readable name: %s\n", shm->readable_name);
    // printf("Semaphore close name: %s\n", shm->close_name);
    // printf("Shared memory name: %s\n", shm->shm_name);

    return read_size;
}

void waitClose(sharedMemADT shm) {
    check_error(sem_wait(shm->close_smh), SEMAPHORE_WAIT_ERROR);
}

void postClose(sharedMemADT shm) {
    check_error(sem_post(shm->close_smh), SEMAPHORE_POST_ERROR);
}

void stop_writing(sharedMemADT shm) {
    shm->to_return[0] = '\0';
}

void close_shared_memory(sharedMemADT shm) {
    check_error(munmap(shm->to_return, shm->max_size), REMOVING_MAPPING_ERROR);
    check_error(sem_close(shm->writable), CLOSING_SEMAPHORE_ERROR);
    check_error(sem_close(shm->readable), CLOSING_SEMAPHORE_ERROR);
    check_error(sem_close(shm->close_smh), CLOSING_SEMAPHORE_ERROR);
    
    // // PROBLEMA CON ELIMINAR POR NOMBRE 
    // check_error(sem_unlink(shm->writable_name), "Error unlinking writable semaphore");
    // check_error(sem_unlink(shm->readable_name), "Error unlinking readable semaphore");
    // check_error(sem_unlink(shm->close_name), "Error unlinking close semaphore");

    // check_error(shm_unlink(shm->shm_name), SHARED_MEMORY_UNLINKING_ERROR);

    free(shm);
}

