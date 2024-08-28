#ifndef SLAVE_H
#define SLAVE_H

#include "error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_SIZE_BUFF 1024

void format_string(char *md5_result, char *hash_result, char *path_result);
void createMD5(char *file_name);

#endif