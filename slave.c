#include "includes/slave.h"
#include "includes/error.h"
//popen() --> pipe open 
int main(int argc, char *argv[]) {
    if (argc < 2) {
        char file_name[256];
        while (1)
        {
            fgets(file_name, sizeof(file_name), stdin);
            file_name[strcspn(file_name, "\n")] = 0; // Remove newline from input

            if(strlen(file_name) > 0){  // a file name was entered
                printf("Procesando archivo %s\n", file_name);
                break;
            }
        }
        
    }

    for (int i = 1; i < argc; i++) {
        //printf("Procesando archivo %s\n", argv[i]);
    }

    return 0;
}

void createMD5(char *file_name) {
    char command[256];

    // create md5sum command to execute in the pipe created
    snprintf(command, sizeof(command), "md5sum %s", file_name);

    // open pipe to execute md5sum command
    FILE *md5 = popen(command, "r");

    if(md5 == NULL){
        check_error(ERROR, PIPE_OPENING_ERROR);
    }

    // get stdout from md5sum
    

    check_error(pclose(md5), PIPE_CLOSING_ERROR);
}