#include "includes/slave.h"
#include "includes/error.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        char file_name[256];
        while (1)
        {
            fgets(file_name, sizeof(file_name), stdin);
            file_name[strcspn(file_name, "\n")] = 0; // Remove newline from input

            if(strlen(file_name) > 0){
                printf("Procesando archivo %s\n", file_name);
                //createChildProcess(1, file_name);
            } else{

            }


        }
        
    }

    for (int i = 1; i < argc; i++) {
        printf("Procesando archivo %s\n", argv[i]);
    }

    return 0;
}