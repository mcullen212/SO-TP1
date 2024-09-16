// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "includes/slave.h"

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);
    char * file_name = NULL;
    size_t fn_size = 0;

    if (argc < 2) {
        while (getline(&file_name, &fn_size, stdin) > 0){
            if (file_name != NULL && strlen(file_name) > 0) {
                // If the last character is a newline, we remove it
                if (file_name[strlen(file_name) - 1] == '\n') {
                    file_name[strlen(file_name) - 1] = '\0';
                }
            }

            create_MD5(file_name);
            free(file_name);
            file_name = NULL;
        }

    }
    free(file_name);
   
    return 0;
}

void format_string(char *md5_result, char *hash_result, char *path_result){
    char *token = strtok(md5_result, " ");
    check_error_token(token);
    strcpy(hash_result, token);
    token = strtok(NULL, " ");
    check_error_token(token);
    token = strtok(token, "\n");
    check_error_token(token);
    strcpy(path_result, token);
}

void create_MD5(char *file_name) {
    char command[256];
    char buff[MAX_SIZE_BUFF];
    char hash_result[MAX_SIZE_BUFF];
    char path_result[MAX_SIZE_BUFF];

    // Create md5sum command to execute in the pipe created
    snprintf(command, sizeof(command), "/usr/bin/md5sum %s", file_name);

    // Open pipe to execute md5sum command
    FILE *pipe = popen(command, "r");
    check_error_pipe(pipe);

    // Read the output from md5sum command
    if (fgets(buff, sizeof(buff), pipe) != NULL) {
        format_string(buff, hash_result, path_result);
        check_error(printf( "NAME: %s - MD5: %s - PID: %d\n", path_result, hash_result, getpid()), WRITE_ERROR);
    }

    check_error(pclose(pipe), PIPE_CLOSING_ERROR);
}