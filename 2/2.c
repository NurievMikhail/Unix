#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	int fd_i = STDIN_FILENO;
	int fd_o = STDOUT_FILENO;

	if (argc >= 2) {
		fd_o = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC);

		if (fd_o == -1) {
			perror("Error with opening");
			return 1;
		}
	}

	char w_buf[BUFFER_SIZE];
	char r_buf[BUFFER_SIZE];
	int readed = 0;
	int length = 0;
	int offset = 0;

	while (1) {
		readed = read(fd_i, r_buf, BUFFER_SIZE);
		if (readed) {
			for (int i = 0; i < readed; i++) {
				if (r_buf[i] == 0) {
					offset++;
				} else if (r_buf[i] != 0) {
					w_buf[length] = r_buf[i];
					length++;
				}				
			}
			if (length > 0) {
				write(fd_o, w_buf, length);
				length = 0;
			}
			if (offset > 0) {
				lseek(fd_o, offset, SEEK_CUR);
				offset = 0;
			}
		} else {
			break;
		}
	}
	close(fd_o);
	return 0;
}
