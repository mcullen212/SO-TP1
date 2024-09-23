// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "includes/sharedMemADT.h"

sharedMemADT *init_shared_memory(char *name, int mode, sharedMemADT *shm) {
	int max_size = MAX_SIZE;

	sprintf(shm->shm_name, "/%s", name);

	// Create shared memory
	int fd = shm_open(shm->shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	check_error(fd, SHARED_MEMORY_OPEN_ERROR);

	check_error(ftruncate(fd, max_size), TRUNCATING_ERROR);

	shm->to_return = mmap(NULL, max_size, mode, MAP_SHARED, fd, 0);

	check_map_error(shm->to_return);

	snprintf(shm->readable_name, MAX_NAME_LENGTH, "/%s_%s", READABLE, name);
	shm->readable = sem_open(shm->readable_name, O_CREAT, S_IRUSR | S_IWUSR, 0);
	check_error_sem(shm->readable);

	// Save data in shared memory ADT
	shm->fd = fd;
	shm->max_size = max_size;
	shm->idx = 0;
	return shm;
}

void write_to_shared_memory(sharedMemADT *shm, const char *buff, int size) {
	int bytes_written = sprintf(&(shm->to_return[shm->idx]), "%s", buff);
	if (bytes_written > 0) {
		shm->idx += bytes_written;
	}
	check_error(sem_post(shm->readable), SEMAPHORE_POST_ERROR); // Indicate data is ready to read
}

int read_from_shared_memory(sharedMemADT *shm, char *buff) {
	check_error(sem_wait(shm->readable), SEMAPHORE_WAIT_ERROR);
	int bytes_read = sprintf(buff, "%s", &(shm->to_return[shm->idx]));
	shm->idx += bytes_read;
	if (shm->to_return[shm->idx - 1] == '+') {
		buff[bytes_read - 1] = '\0';
		return -1;
	}
	return bytes_read;
}

void post_readable(sharedMemADT shm) {
	check_error(sem_post(shm.readable), SEMAPHORE_POST_ERROR);
}

void close_shared_memory(sharedMemADT shm) {
	munmap(shm.to_return, shm.max_size);
	close(shm.fd);
	sem_close(shm.readable);
}

void destroy_shared_memory(sharedMemADT shm) {
	check_error(sem_unlink(shm.readable_name), SEMAPHORE_DESTROY_ERROR);
	check_error(shm_unlink(shm.shm_name), SHARED_MEMORY_UNLINKING_ERROR);
}
