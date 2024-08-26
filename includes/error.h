#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>

#define ERROR -1
#define PIPE_ERROR "Error: creating pipe"
#define FORK_ERROR "Error: creating child process"
#define EXEC_ERROR "Error: executing command"
#define WAITPID_ERROR "Error: waiting for child process"
#define NO_FILES "Error: no files to process"

void check_error(int code, char *msg);

#endif