#include "includes/error.h"

void check_error(int code, char *msg) {
    if(code == -1) {
        perror(msg);
        exit(errno);
    }
    return;
}