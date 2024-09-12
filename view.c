// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "includes/view.h"

int main(int argc, char * argv[]){
    setvbuf(stdin, NULL, _IONBF, 0);

    pid_t pid;
    int amount_of_files;

    if(argc == 3){
        amount_of_files = strtol(argv[2], NULL, 10);
        pid = atoi(argv[1]);
    } else if(argc == 1){
        char pid_aux[MAX_PID];
        char amount_aux[BUFFER_SIZE];
        if(scanf("%s %s", pid_aux, amount_aux) == 2){
            amount_of_files = strtol(amount_aux, NULL, 10);
            pid = strtol(pid_aux, NULL, 10);
            //printf("Shared memory: %s %s\n", sharedmem, is_creator_);
        } else {
            perror(INIT_VIEW_ERROR);
            exit(errno);
        }
    }
    
    sharedMemADT shm = init_shared_memory(pid, amount_of_files, PROT_READ);
    int bytes_read = 1;
    char to_return[BUFFER_SIZE];

    wait_close(shm);

    while(bytes_read != -1){
        bytes_read = read_from_shared_memory(shm, to_return);
        if(bytes_read != -1){
            printf("%s", to_return);
        }
    }

    post_close(shm);
    close_shared_memory(shm); 

    return 0; 
}

