#include "includes/view.h"

int main(int argc, char * argv[]){
    pid_t pid;
    int amount_of_files;
    char to_return[256];

  

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
    
    sharedMemADT shm = init_shared_memory(pid, amount_of_files);
    
    //while( read_from_shared_memory(shm, to_return) > 0){
    read_from_shared_memory(shm, to_return);
        printf("%s\n", to_return);
   // }

 
    close_shared_memory(shm);


    exit(1);
}

