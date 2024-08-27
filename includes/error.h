#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>

#define ERROR -1
#define PIPE_CREATING_ERROR "Error: creating pipe"
#define PIPE_OPENING_ERROR "Error: opening from pipe"
#define PIPE_CLOSING_ERROR "Error: closing pipe"
#define FORK_ERROR "Error: creating child process"
#define EXEC_ERROR "Error: executing command"
#define WAITPID_ERROR "Error: waiting for child process"
#define NO_FILES "Error: no files to process"

void check_error(int code, char *msg);

#endif