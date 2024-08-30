#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR -1
#define PIPE_CREATING_ERROR "Error: creating pipe"
#define PIPE_OPENING_ERROR "Error: opening from pipe"
#define PIPE_CLOSING_ERROR "Error: closing pipe"
#define FORK_ERROR "Error: creating child process"
#define EXEC_ERROR "Error: executing command"
#define WAITPID_ERROR "Error: waiting for child process"
#define NO_FILES "Error: no files to process"
#define WRITE_ERROR "Error: when writing output in pipe"
#define EXECVE_ERROR "Error: Executing slave"
#define DUP_ERROR "Error: Duplicating file descriptor"
#define READ_ERROR "Error: Reading pipe"

void check_error(int code, char *msg);

#endif