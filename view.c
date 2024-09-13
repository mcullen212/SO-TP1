// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "includes/view.h"

int main(int argc, char * argv[]){
    setvbuf(stdin, NULL, _IONBF, 0);

    char * shared_memory_name;
    
    if(argc == 2){
        shared_memory_name = argv[1];
    } else if(argc == 1){
        char shared_memory_name_aux[MAX_NAME_LENGTH];
        
        if(scanf("%s", shared_memory_name_aux) == 1){
            shared_memory_name = shared_memory_name_aux;
        } else {
            perror(INIT_VIEW_ERROR);
            exit(errno);
        }
    }
    
    sharedMemADT shm = init_shared_memory(shared_memory_name, PROT_READ);

    wait_close(shm);

    int bytes_read = 1;
    char to_return[BUFFER_SIZE];
    
    while(bytes_read != -1){
        bytes_read = read_from_shared_memory(shm, to_return);
        if(bytes_read == -1){
            break;
        
        }
         printf("%s", to_return);     
    }
    
    post_close(shm);
    close_shared_memory(shm); 
    
    return 0; 
}

