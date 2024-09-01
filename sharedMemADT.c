#include "includes/sharedMemADT.h"

typedef struct sharedMemCDT {
    int fd;
    char *to_return;
    size_t size_to_return;
    sem_t semaphore;
} *sharedMemADT;

sharedMemADT init_shared_memory(int is_creator) {
    sharedMemADT data;

    // Create or open the shared memory
    if (is_creator) {
        // Unlink any existing shared memory  
        shm_unlink(SHM_NAME);
        
        // Open shared memory with create and read-write permissions
        int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        check_error(fd, SHARED_MEMORY_OPEN_ERROR);
        
        // Set the size of the shared memory
        check_error(ftruncate(fd, SHM_SIZE), TRUNCATING_ERROR);
        
        // Map the shared memory  into the address space
        data = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            check_error(ERROR, MAPPING_ERROR);
        }

        // Initialize semaphore
        check_error(sem_init(&(data->semaphore), 1, 1), SEMAPHORE_FAILED_ERROR);
        
        data->fd = fd;
    } else {
        // Open existing shared memory
        int fd = shm_open(SHM_NAME, O_RDWR, 0);
        check_error(fd, SHARED_MEMORY_OPEN_ERROR);

        // Map the shared memory into the address space
        data = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED) {
            check_error(ERROR, MAPPING_ERROR);
        }
        
        data->fd = fd;
    }

    return data;
}

void write_to_shared_memory(sharedMemADT data, const char* string) {
    // Lock the semaphore
    sem_wait(&(data->semaphore));

    // Write data to shared memory
    snprintf(data->to_return, data->size_to_return, "%s", string);

    // Unlock the semaphore
    sem_post(&(data->semaphore));
}

void close_shared_memory(sharedMemADT shm) {
    // Close the shared memory object
    shm_unlink(SHM_NAME);
    close(shm->fd);
    sem_close(&(shm->semaphore));
}

void read_from_shared_memory(sharedMemADT data) {
    // Lock the semaphore
    sem_wait(&(data->semaphore));

    // Read data from shared memory
    data->size_to_return = printf("%s", data->to_return);

    // Unlock the semaphore
    sem_post(&(data->semaphore));
}

void cleanup_shared_memory(sharedMemADT data) {
    // Remove the mapping of the shared memory 
    check_error(munmap(data, SHM_SIZE), REMOVING_MAPPING_ERROR);

    // Remove the name shared memory object
    check_error(shm_unlink(SHM_NAME), REMOVING_NAME_SHM_ERROR);

    // Destroy the semaphore
    check_error(sem_destroy(&(data->semaphore)), DESTROYING_SEMAPHORE_ERROR);
}