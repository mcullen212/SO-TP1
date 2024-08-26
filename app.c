#include <stdio.h>
#include <stdlib.h>
#include "includes/error.h"
#include "includes/app.h"

void * createSharedMemory(size_t size) {
    // Create shared memory
}

int createChildProcess(int amount, char **file_names) {
    pid_t pid = fork();

    check_error(pid, FORK_ERROR);

    if (pid == 0) {
        // Child process
        
        return 0;
    } else {
        // Parent process
        
    }

    return pid;
}

//create a result .txt file with the name of the file and the md5 hash
// FileName: .txt -> md5: hash -> PID: pid 
// order of arrival
int main(int argc, char *argv[]) {
    if(argc<2){
        check_error(ERROR, NO_FILES);
    }
    
    for (int i = 1; i < argc; i++) {
        printf("Procesando archivo %s\n", argv[i]);
    }

    return 0;
}