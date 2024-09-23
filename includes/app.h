#ifndef APP_H
#define APP_H

#include "error.h"
#include "sharedMemADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define WRITE_END_FD 1
#define READ_END_FD 0

#define MIN_SLAVES 5
#define MIN_FILES_PER_SLAVE 1

#define AMOUNT_OF_SLAVES(files) ((int) ((files) * 0.1))
#define FILES_PER_SLAVE 2

#define NEWLINE "\n"
#define BUFFER_SIZE 2024
#define DIM_BUFFER 256

#define SLEEP_TIME 4

#define MAX_PID 10000

void writeInPipe(int fd, char *buff);
void redirect_pipe(int open_fd, int close_fd, FILE *stream);
int create_slave_process(int *fd_in_slave, int *fd_out_slave, int current_slave);
void get_slaves(int amount_of_files, int *amount_of_slaves, int *amount_of_files_per_slave);

#endif