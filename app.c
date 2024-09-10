// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "includes/app.h"

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

    }

    return pid;
}

void get_slaves(int amount_of_files, int *amount_of_slaves, int *amount_of_files_per_slave) {
    if(amount_of_files < MIN_SLAVES){
        *amount_of_slaves = amount_of_files;
        *amount_of_files_per_slave = MIN_FILES_PER_SLAVE;
    }
    else if(AMOUNT_OF_SLAVES(amount_of_files) < MIN_SLAVES){
        *amount_of_slaves = MIN_SLAVES;
        *amount_of_files_per_slave = 0.2*amount_of_files;
    }
    else{
        *amount_of_slaves = AMOUNT_OF_SLAVES(amount_of_files);
        *amount_of_files_per_slave = FILES_PER_SLAVE;
    }
}

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);
    if(argc<2){
        check_error(ERROR, NO_FILES);
    }
    
    int amount_of_files = argc - 1;
    int amount_of_slaves;
    int amount_of_files_per_slave;

    printf("%d %d\n", getpid(), amount_of_files);
    

    get_slaves(amount_of_files, &amount_of_slaves, &amount_of_files_per_slave);

    pid_t slave_pids[amount_of_slaves];
    int fd_in_slave[amount_of_slaves];
    int fd_out_slave[amount_of_slaves];

    // Create shared memory 
    sharedMemADT shared_memory = init_shared_memory(getpid(), amount_of_files, PROT_WRITE);
    sleep(SLEEP_TIME);
    int current_slaves = 0;
    
    while(current_slaves < amount_of_slaves){
        slave_pids[current_slaves] = create_slave_process(fd_in_slave, fd_out_slave, current_slaves);
        current_slaves++;
    }

    // Open the file to write the results
    FILE *results = fopen("results.txt", "w");
    if(results == NULL){
        check_error(ERROR, FILE_ERROR);
    }

    FILE *slave_output[amount_of_slaves];
    FILE *slave_input[amount_of_slaves];
    // Update pipe input and output streams to use shared memory
    // Save the file pointer of each slave
    for (int i = 0; i < amount_of_slaves; i++) {
        slave_output[i] = fdopen(fd_out_slave[i], "r");
        slave_input[i] = fdopen(fd_in_slave[i], "w");
        setvbuf(slave_output[i], NULL, _IONBF, 0);
        setvbuf(slave_input[i], NULL, _IONBF, 0);
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
        
        char buffer[256];
        // Go through all the slaves and check if they have finished processing the file
        for (int i = 0; i < amount_of_slaves; i++) {
            if (FD_ISSET(fd_out_slave[i], &temp_fds)) { // Check if the fd is still present, if not send a new file 
                ssize_t bytes_read = read(fd_out_slave[i], buffer, 256);

                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    files_processed++;
                    write_to_shared_memory(shared_memory, buffer, bytes_read);
                    fprintf(results, "%s", buffer);

                    // Check if all the files were processed 
                    if (files_distributed < amount_of_files) {
                        writeInPipe(fd_in_slave[i], argv[files_distributed + 1]);
                        files_distributed++;
                    }
                }
            }
        }
    } 
    ready(shared_memory);
    
    // Close the file
    fclose(results);

    // Cleanup shared memory
    wait_close(shared_memory);
    close_shared_memory(shared_memory); 

    // Close file descriptors
    for(int i = 0; i < amount_of_slaves; i++) {
        fclose(slave_output[i]);
        fclose(slave_input[i]);
        close(fd_in_slave[i]);
        close(fd_out_slave[i]);
    }

    // Wait for all the slaves to finish
    for(int i = 0; i < amount_of_slaves; i++) {
        check_error(waitpid(slave_pids[i], NULL, 0), WAITPID_ERROR);
    }

    return 0;
}
