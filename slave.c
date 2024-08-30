#include "includes/slave.h"

int main(int argc, char *argv[]) {
    char * file_name = NULL;
    size_t fn_size = 0;

    if (argc < 2) {
        while (getline(&file_name, &fn_size, stdin) > 0){
                file_name[strlen(file_name) - 1] = '\0'; // remove newline 
                createMD5(file_name);
                free(file_name);
                file_name = NULL;

        }

    }
    free(file_name);
   
    exit(1);
}

void format_string(char *md5_result, char *hash_result, char *path_result){
    char *token = strtok(md5_result, " ");
    strcpy(hash_result, token);
    token = strtok(NULL, " ");
    strcpy(path_result, strtok(token, "\n"));
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
        format_string(buff, hash_result, path_result);
        char toRet[MAX_SIZE_BUFF * 3];
        int dim = snprintf(toRet, sizeof(toRet), "NAME: %s - MD5: %s - PID: %d\n", path_result, hash_result, getpid()); // format the output
        check_error((int) write(fileno(stdout), toRet, dim), WRITE_ERROR); // write in standard output
    }

    check_error(pclose(pipe), PIPE_CLOSING_ERROR);
}