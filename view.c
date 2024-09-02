#include "includes/view.h"

int main(int argc, char * argv[]){
    pid_t pid;
    int amount_of_files;
    char to_return[256];

    printf("argc: %d\n", argc);

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
    printf("PID: %d\n", pid);
    printf("Amount of files: %d\n", amount_of_files);
    sharedMemADT shm = init_shared_memory(pid, 0, amount_of_files);
    
    read_from_shared_memory(shm, to_return);
    printf("paso el read\n");
    
    printf("%s\n", to_return);

 
    close_shared_memory(shm);


    exit(1);
}

