#include "includes/app.h"

// void * create_shared_memory(size_t size) {
//     // Create shared memory
// }

void writeInPipe(int fd, char * buff) {
    write(fd, buff, strlen(buff));
    write(fd, NEWLINE, sizeof(char));
}

void redirect_pipe(int open_fd, int close_fd, FILE *stream) {
    check_error(close(close_fd),PIPE_CLOSING_ERROR); // close write end of the pipe
    check_error(dup2(open_fd, fileno(stream)), DUP_ERROR); // duplicate fd to position wanted 
    check_error(close(open_fd), PIPE_CLOSING_ERROR); //close duplicated fd 
}

int create_slave_process(int *fd_in_slave, int *fd_out_slave, int current_slave) {
    
    int master_to_slave[2]; // input from master to slave
    int slave_to_master[2]; // output from slave to master

    // Create pipes
    check_error(pipe(master_to_slave), PIPE_CREATING_ERROR);
    check_error(pipe(slave_to_master), PIPE_CREATING_ERROR);

    pid_t pid = fork();
    check_error(pid, FORK_ERROR);         

    if (pid == 0) { // slave process
        char * slave_argv[] = {"./slave", NULL};
        char * slave_env[] = {NULL};

        //redirecting stdin to the master_to_slave
        // master input for slave process
        // master Stdint slave
        redirect_pipe(master_to_slave[READ_END_FD], master_to_slave[WRITE_END_FD], stdin);

        //redirecting stdout to the slave_to_master
        // slave output for master process 
        // slave Stdout master
        redirect_pipe(slave_to_master[WRITE_END_FD], slave_to_master[READ_END_FD], stdout);

        //closing unused ends of the pipe of "parent" slaves
        for(int i = 0; i < current_slave; i++){
            close(fd_in_slave[i]);
            close(fd_out_slave[i]);
        }
        
        check_error(execve("./slave", slave_argv, slave_env), EXECVE_ERROR);
        
    } else { // Parent process
        //closing unused slave process ends of the pipe
        close(master_to_slave[READ_END_FD]); // close write end of the pipe
        close(slave_to_master[WRITE_END_FD]); // close read end of the pipe

        // master -> slave
        fd_in_slave[current_slave] = master_to_slave[WRITE_END_FD];
        fd_out_slave[current_slave] = slave_to_master[READ_END_FD];

        //closing pipes before finishing
        close(master_to_slave[READ_END_FD]); // close read end of the pipe
        close(slave_to_master[WRITE_END_FD]); // close write end of the pipe
    }

    return pid;
}

void get_slaves(int amount_of_files, int *amount_of_slaves, int *amount_of_files_per_slave) {
    if(AMOUNT_OF_SLAVES(amount_of_files) < MIN_SLAVES){
        *amount_of_slaves = MIN_SLAVES;
        *amount_of_files_per_slave = MIN_FILES_PER_SLAVE;
    }else{
        *amount_of_slaves = AMOUNT_OF_SLAVES(amount_of_files);
        *amount_of_files_per_slave = FILES_PER_SLAVE;
    }
}

//create a result .txt file with the name of the file and the md5 hash
// FileName: .txt -> md5: hash -> PID: pid 
// order of arrival
int main(int argc, char *argv[]) {
    if(argc<2){
        check_error(ERROR, NO_FILES);
    }

    printf("Amount of files: %d\n", argc - 1);
    int amount_of_files = argc - 1;
    int amount_of_slaves;
    int amount_of_files_per_slave;
    

    get_slaves(amount_of_files, &amount_of_slaves, &amount_of_files_per_slave);

    pid_t slave_pids[amount_of_slaves];
    int fd_in_slave[amount_of_slaves];
    int fd_out_slave[amount_of_slaves];

    int current_slaves = 0;
    
    for(; current_slaves < amount_of_slaves; current_slaves++){
        slave_pids[current_slaves] = create_slave_process(fd_in_slave, fd_out_slave, current_slaves);
    }

    fd_set readfds;
    int files_distributed = 0;
    int files_processed = 0;

    // Set the file descriptors 
    FD_ZERO(&readfds);

    // Set the inicial amount of files to be processed by the slaves
    for(int file_idx = 0; file_idx < amount_of_files_per_slave*amount_of_slaves ; file_idx++){
        writeInPipe(fd_in_slave[file_idx % amount_of_slaves], argv[file_idx + 1]);
        files_distributed++;
        
        if(file_idx < amount_of_slaves){
            FD_SET(fd_out_slave[file_idx], &readfds);
        }
        
    }

    while (files_processed < amount_of_files) {
        fd_set temp_fds = readfds; // Copy the original grupo of descriptors

        // Wait for any of the file descriptors to be ready
        check_error(select(FD_SETSIZE, &temp_fds, NULL, NULL, NULL), SELECT_ERROR);
        

        // Go through all the slaves and check if they have finished processing the file
        for (int i = 0; i < amount_of_slaves; i++) {
            if (FD_ISSET(fd_out_slave[i], &temp_fds)) { // Check if the fd is still present, if not send a new file 
                char buffer[256];
                ssize_t bytes_read = read(fd_out_slave[i], buffer, sizeof(buffer) - 1);

                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    files_processed++;

                    // Check if all the files were processed 
                    if (files_distributed < amount_of_files) {
                        writeInPipe(fd_in_slave[i], argv[files_distributed + 1]);
                        files_distributed++;
                    }
                }
            }
        }
    } 

    printf("Master PID: %d\n", getpid());
    return 0;
}


    // // for(int slave_idx = 0; slave_idx < amount_of_slaves; slave_idx++) {
    // //     setvbuf(fdopen(fd_in_slave[slave_idx], "w"), NULL, _IONBF, 0);
    // //     setvbuf(fdopen(fd_out_slave[slave_idx], "r"), NULL, _IONBF, 0);
    // // }

    // int files_distributed = 0;

    // for(int file_idx = 0; file_idx < amount_of_files; file_idx++){
    //     writeInPipe(fd_in_slave[file_idx % amount_of_slaves], argv[file_idx + 1]);
    // }

    // while(files_distributed < amount_of_files){
        
    // }

    // for(int slave_idx = 0; slave_idx < amount_of_files; slave_idx++) {   
    //     char buffer[256];
    //     ssize_t bytes_read = read(fd_out_slave[slave_idx % amount_of_slaves], buffer, sizeof(buffer) - 1); // Lee lo que el esclavo envía
    //     if (bytes_read > 0) {
    //         buffer[bytes_read] = '\0'; // Asegura que el buffer termine en null para ser tratado como string
    //         printf("%s", buffer); // Muestra el resultado en la pantalla
    //     }
    // }

   
