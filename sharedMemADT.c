#include "includes/sharedMemADT.h"

typedef struct sharedMemCDT {
    char name[255];
    int fd;
    char *to_return;
    size_t max_size; // Max size
    size_t actual_size; // Actual size
    sem_t semaphore;
} *sharedMemADT;

sharedMemADT init_shared_memory(pid_t pid, int is_creator, int amount_of_files) {
    sharedMemADT data;

    char name[255];
    sprintf(name, "%s_%d", SHM_NAME, pid);
    // Create or open the shared memory
    if (is_creator) {
        // Unlink any existing shared memory  
        shm_unlink(name);

        // Open shared memory with create and read-write permissions
        int fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        check_error(fd, SHARED_MEMORY_OPEN_ERROR);

        size_t max_size = SIZE_STR*amount_of_files;
        
        // Set the size of the shared memory
        check_error(ftruncate(fd, max_size), TRUNCATING_ERROR);

        // Map the shared memory  into the address space
        data = mmap(0, max_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            check_error(ERROR, MAPPING_ERROR);
        }
        data->fd = fd;
        data->max_size = max_size;
        strcpy(data->name, name);
        data->actual_size = 0;
        // Initialize semaphore
        check_error(sem_init(&(data->semaphore), 1, 1), SEMAPHORE_FAILED_ERROR);
    } else {
        // Open existing shared memory
        int fd = shm_open(name, O_CREAT | O_RDWR, 0);
        check_error(fd, SHARED_MEMORY_OPEN_ERROR);

        size_t max_size = SIZE_STR*amount_of_files;

        check_error(ftruncate(fd, max_size), TRUNCATING_ERROR);

        
        // Map the shared memory into the address space
        data = mmap(0, max_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            check_error(ERROR, MAPPING_ERROR);
        }
        data->fd = fd;
        strcpy(data->name, name);
        data->max_size = max_size;
        data->actual_size = 0;
    }
    return data;
}

void write_to_shared_memory(sharedMemADT data, const char* string) {
    // Lock the semaphore
    //sem_wait(&(data->semaphore));

    // Write data to shared memory
    snprintf(data->to_return, data->actual_size, "%s", string);

    // Unlock the semaphore
    sem_post(&(data->semaphore));
}

void close_shared_memory(sharedMemADT shm) {
    // Close the shared memory object
    check_error(shm_unlink(shm->name), REMOVING_NAME_SHM_ERROR);
    check_error(close(shm->fd), PIPE_CLOSING_ERROR);
    check_error(sem_destroy(&(shm->semaphore)), DESTROYING_SEMAPHORE_ERROR);
    munmap(shm, shm->max_size);
}

int read_from_shared_memory(sharedMemADT data, char * buff) {
    // Lock the semaphore
    printf("Antes del sem_wait\n");
    sleep(4);
    for(int i = 0; i < data->actual_size; i++){
        printf("%c", data->to_return[i]);
    }
   
    
    sem_wait(&(data->semaphore));
    // Read data from shared memory
    snprintf(buff, data->actual_size, "%s", data->to_return);
    // data->actual_size = strlen(data->to_return);
    // strcpy(buff, data->to_return);

    // Unlock the semaphore
    sem_post(&(data->semaphore));
    
    return data->actual_size;
}

void cleanup_shared_memory(sharedMemADT data) {
    // Remove the mapping of the shared memory 
    //check_error(munmap(data, data->actual_size), REMOVING_MAPPING_ERROR);

    // Remove the name shared memory object
    check_error(shm_unlink(data->name), REMOVING_NAME_SHM_ERROR);

    check_error(close(data->fd), PIPE_CLOSING_ERROR);

    // Destroy the semaphore
    check_error(sem_destroy(&(data->semaphore)), DESTROYING_SEMAPHORE_ERROR);
    free(data);
}

void printName(sharedMemADT data){
    printf("%s", data->name);
}

