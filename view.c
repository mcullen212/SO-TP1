// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "includes/view.h"

int main(int argc, char *argv[]) {
	setvbuf(stdin, NULL, _IONBF, 0);

	char shared_memory_name[MAX_NAME_LENGTH];

	if (argc == 2) {
		strcpy(shared_memory_name, argv[1]);
	}
	else if (argc == 1) {
		if (scanf("%254s", shared_memory_name) != 1) {
			perror(INIT_VIEW_ERROR);
			exit(errno);
		}
	}
	else {
		perror("Invalid argument count");
		exit(EXIT_FAILURE);
	}

	sharedMemADT shm;
	init_shared_memory(shared_memory_name, PROT_WRITE, &shm);

	int bytes_read = 1;
	char to_return[BUFFER_SIZE] = {0};

	while (bytes_read != -1) {
		bytes_read = read_from_shared_memory(&shm, to_return);
		printf("%s", to_return);
	}

	close_shared_memory(shm);

	return 0;
}
