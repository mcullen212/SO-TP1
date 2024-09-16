// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "includes/error.h"

void check_error(int code, char *msg) {
    if (code == -1)
    {
        perror(msg);
        exit(errno);
    }
    return;
}

void check_error_token(char *token) {
    if (token == NULL)
    {
        check_error(ERROR, MD5_ERROR);
    }
}

void check_error_sem(sem_t *sem) {
    if (sem == SEM_FAILED)
    {
        check_error(ERROR, SEMAPHORE_OPENING_ERROR);
    }
}

void check_map_error(void *ptr) {
    if (ptr == MAP_FAILED)
    {
        check_error(ERROR, MAPPING_ERROR);
    }
}

void check_error_file(FILE *file) {
    if (file == NULL)
    {
        check_error(ERROR, FILE_ERROR);
    }
}

void check_error_pipe(FILE *pipe) {
    if (pipe == NULL)
    {
        check_error(ERROR, PIPE_OPENING_ERROR);
    }
}