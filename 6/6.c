#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READ_TOOL "cat"
#define WRITE_TOOL "vim"
#define EXTENSION ".lck"

int main(int argc, char* argv[]) {
	if (argc < 3 || (strcmp(argv[2], "write") != 0 && strcmp(argv[2], "read") != 0)) {
		printf("As arguments, you must write the file name and \"write\" or \"read\"\n");
		return 1;
	}

	char* lock_file = malloc(sizeof(char) * (strlen(argv[1]) + strlen(EXTENSION)));
	if (lock_file == NULL){
		printf("Error while allocating\n");
		return 1;
	}
	sprintf(lock_file, "%s%s", argv[1], EXTENSION);

	char* tool;
	if (strcmp(argv[2], "read") == 0) {
		tool = READ_TOOL;
	} else {
		tool = WRITE_TOOL;
	}
	char* command = malloc(sizeof(char) * (strlen(argv[1]) + strlen(tool)));
	if (command == NULL) {
		printf("Error while allocating\n");
		return 1;
	}
	sprintf(command, "%s %s", tool, argv[1]);

	FILE* lock;
	while ((lock = fopen(lock_file, "r")) != NULL) {
		fclose(lock);
		sleep(1);
	}

	lock = fopen(lock_file, "w");
	fprintf(lock, "%d\n", getpid());
	fprintf(lock, "%s\n", argv[2]);
	fclose(lock);
	system(command);
	remove(lock_file);

	return 0;
}
