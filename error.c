// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "includes/error.h"

void check_error(int code, char *msg) {
    if(code == -1) {
        perror(msg);
        exit(errno);
    }
    return;
}