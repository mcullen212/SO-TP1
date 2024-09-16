// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "includes/view.h"

int main(int argc, char * argv[]){
    setvbuf(stdin, NULL, _IONBF, 0);

    char *shared_memory_name = NULL; 

    if (argc == 2) {
       
        shared_memory_name = argv[1];
    } else if (argc == 1) {
        
        shared_memory_name = malloc(MAX_NAME_LENGTH * sizeof(char));

        if (shared_memory_name == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }

        char shared_memory_name_aux[MAX_NAME_LENGTH];
        
        if (scanf("%254s", shared_memory_name_aux) == 1) {
            strcpy(shared_memory_name, shared_memory_name_aux);
        } else {
            perror(INIT_VIEW_ERROR);
            free(shared_memory_name);  // Libera la memoria si hubo un error
            exit(errno);
        }
    } else {
        perror("Invalid argument count");
        exit(EXIT_FAILURE);
    }
    
    sharedMemADT shm = init_shared_memory(shared_memory_name, PROT_READ);

    int bytes_read = 1;
    char to_return[BUFFER_SIZE] = {0};
    
    while (bytes_read != -1) {
        bytes_read = read_from_shared_memory(shm, to_return);
        printf("%s", to_return);     
    }

    close_shared_memory(shm);
    if (argc == 1) {
        // Solo liberamos si usamos malloc
        free(shared_memory_name);
    }
    free(shm);
    
    return 0; 
}
