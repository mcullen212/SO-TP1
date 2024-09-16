#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>

#define ERROR (-1)
#define EXIT_ERROR "Error: Exiting"
#define PIPE_CREATING_ERROR "Error: Creating pipe"
#define PIPE_OPENING_ERROR "Error: Opening from pipe"
#define PIPE_CLOSING_ERROR "Error: Wlosing pipe"
#define PIPE_DUP_ERROR "Error: Duplicating pipe"
#define MD5_ERROR "Error: Splitting MD5 result"
#define FORK_ERROR "Error: Wreating child process"
#define EXEC_ERROR "Error: Wxecuting command"
#define WAITPID_ERROR "Error: Waiting for child process"
#define NO_FILES "Error: No files to process"
#define WRITE_ERROR "Error: When writing output in pipe"
#define EXECVE_ERROR "Error: Executing slave"
#define DUP_ERROR "Error: Duplicating file descriptor"
#define READ_ERROR "Error: Reading pipe"
#define SELECT_ERROR "Error: Monitoring file descriptors with select"
#define SHM_NULL_ERROR "Error: Shared memory was not allocated correctly"
#define MAPPING_ERROR "Error: Mapping shared memory"
#define SHARED_MEMORY_OPEN_ERROR "Error: Opening/Creating shared memory"
#define SHARED_MEMORY_CLOSE_ERROR "Error: Closing shared memory"
#define SHARED_MEMORY_UNLINKING_ERROR "Error: Unlinking shared memory"
#define SEMAPHORE_OPENING_ERROR "Error: Initializing semaphore"
#define SEMAPHORE_WAIT_ERROR "Error: Waiting for semaphore"
#define SEMAPHORE_POST_ERROR "Error: Posting semaphore"
#define SEMAPHORE_UNLINKING_ERROR "Error: Unlinking semaphore"
#define TRUNCATING_ERROR "Error: Truncating file size"
#define REMOVING_MAPPING_ERROR "Error: Removing mapping of the existing shared memory"
#define REMOVING_NAME_SHM_ERROR "Error: Removing name of the shared memory "
#define CLOSING_SEMAPHORE_ERROR "Error: Closing semaphore"
#define FILE_ERROR "Error: Opening file"
#define FDOPEN_ERROR "Error: Opening file descriptor"
#define INIT_VIEW_ERROR "Error: Initializing view"
#define SEMAPHORE_DESTROY_ERROR "Error: Destroying semaphore"

void check_error(int code, char *msg);
void check_error_token(char *token);
void check_error_sem(sem_t *sem);
void check_map_error(void *ptr);
void check_error_file(FILE *file);
void check_error_shm(void * shm);
void check_error_pipe(FILE *pipe);


#endif