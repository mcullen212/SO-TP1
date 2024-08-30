#include "includes/sharedMemADT.h"

typedef struct sharedMemCDT {
    char *toReturn;
    sem_t semaphore;
}sharedMemCDT;

sharedMemADT init_shared_memory(int is_creator) {
    int shm_fd;
    struct sharedMemCDT* data;

    // Create or open the shared memory object
    if (is_creator) {
        sharedMemADT new_shm = malloc(SHM_SIZE);
        shm_unlink(SHM_NAME);
        shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        check_error(shm_fd, SHARED_MEMORY_OPEN_ERROR);
        
        check_error(ftruncate(shm_fd, SHM_SIZE), TRUNCATING_ERROR);
    } else {
        shm_fd = shm_open(SHM_NAME, O_RDWR);
        check_error(shm_fd, EXIT_FAILURE);
    }

    // Map the shared memory object to the address space
    data = sharedMemADT mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    check_error(data, MAPPING_ERROR);

    if (is_creator) {
        // Initialize semaphore
        check_error(sem_init(&(data->semaphore), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR), SEMAPHORE_FAILED_ERROR);
    }

    return data;
}

void write_to_shared_memory(sharedMemADT data, const char* string) {
    // Lock the semaphore
    sem_wait(&(data->semaphore));

    // Write data to shared memory
    snprintf(data->toReturn, sizeof(data->toReturn), "%s", string);

    // Unlock the semaphore
    sem_post(&(data->semaphore));
}

void read_from_shared_memory(sharedMemADT data) {
    // Lock the semaphore
    sem_wait(&(data->semaphore));

    // Read data from shared memory
    printf("%s\n", data->toReturn);

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