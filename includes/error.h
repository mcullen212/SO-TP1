#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR -1
#define PIPE_CREATING_ERROR "Error: Creating pipe"
#define PIPE_OPENING_ERROR "Error: Opening from pipe"
#define PIPE_CLOSING_ERROR "Error: Wlosing pipe"
#define FORK_ERROR "Error: Wreating child process"
#define EXEC_ERROR "Error: Wxecuting command"
#define WAITPID_ERROR "Error: Waiting for child process"
#define NO_FILES "Error: No files to process"
#define WRITE_ERROR "Error: When writing output in pipe"
#define EXECVE_ERROR "Error: Executing slave"
#define DUP_ERROR "Error: Duplicating file descriptor"
#define READ_ERROR "Error: Reading pipe"
#define SELECT_ERROR "Error: Monitoring file descriptors with select"

void check_error(int code, char *msg);

#endif