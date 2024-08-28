#include "includes/slave.h"
#include "includes/error.h"
//popen() --> pipe open 

void format_string(char *md5_result, char *hash_result, char *path_result){
    char *token = strtok(md5_result, " ");
    strcpy(hash_result, token);
    token = strtok(NULL, " ");
    strcpy(path_result, strtok(token, "\n"));

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        char * file_name = NULL;
        size_t fn_size = 0;
        while (1)
        {
            getline(&file_name, &fn_size, stdin);
            if(strlen(file_name) > 0){  // a file name was entered
                file_name[strlen(file_name) - 1] = '\0'; // remove newline 
                createMD5(file_name);
                break;
            }
        }
        free(file_name);
    }

    for (int i = 1; i < argc; i++) {
        //printf("Procesando archivo %s\n", argv[i]);
    }

    return 0;
}

void createMD5(char *file_name) {
    char command[256];
    char buff[MAX_SIZE_BUFF];
    char hash_result[MAX_SIZE_BUFF];
    char path_result[MAX_SIZE_BUFF];

    // create md5sum command to execute in the pipe created
    snprintf(command, sizeof(command), "/usr/bin/md5sum %s", file_name);

    // open pipe to execute md5sum command
    FILE *pipe = popen(command, "r");
    if(!pipe){
        check_error(ERROR, PIPE_OPENING_ERROR);
    }

        // read the output from md5sum command
        if (fgets(buff, sizeof(buff), pipe) != NULL) {
            pclose(pipe);
            format_string(buff, hash_result, path_result);
            char toRet[MAX_SIZE_BUFF*3];
            int dim = snprintf(toRet, sizeof(toRet), "%d-%s %s\n", getpid(), hash_result, path_result); // format the output
            check_error((int) write(stdout, toRet, dim), WRITE_ERROR); // write in standard output
        }

    check_error(pclose(pipe), PIPE_CLOSING_ERROR);
}

void check_error(int code, char *msg) {
    if(code == -1) {
        perror(msg);
        exit(errno);
    }
    return;
}