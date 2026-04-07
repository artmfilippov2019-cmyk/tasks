#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "fifo_chat.h"

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Ошибка: неверное количество аргументов\n");
		return 1;
	}

	fifo_path = argv[1];

	if (mkfifo(fifo_path, 0666) == -1) {
		if (errno != EEXIST) {
			perror("Ошибка создания FIFO");
			return 1;
		}
	}

	fifo_fd = open(fifo_path, O_RDWR | O_NONBLOCK);
	if (fifo_fd == -1) {
		perror("Ошибка открытия FIFO");
		return 1;
	}

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	char prompt_in, prompt_out;
	if (getpid() % 2 == 0) {
		prompt_in = '<';
		prompt_out = '>';
	}
	else {
		prompt_in = '>';
		prompt_out = '<';
	}

	char read_buf[BUFFER_SIZE];
	char write_buf[BUFFER_SIZE];
	int write_len = 0;

	fprintf(stderr, "%c ", prompt_in);
	fflush(stderr);

	fd_set read_fds;
	int max_fd = (fifo_fd > STDIN_FILENO) ? fifo_fd : STDIN_FILENO;

	while (1) {
		FD_ZERO(&read_fds);
		FD_SET(STDIN_FILENO, &read_fds);
		FD_SET(fifo_fd, &read_fds);

		if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
			break;
		}

		if (FD_ISSET(STDIN_FILENO, &read_fds)) {
			char c;
			if (read(STDIN_FILENO, &c, 1) > 0) {
				if (c == '\n') {
					write_buf[write_len] = '\0';

					char msg[BUFFER_SIZE + 1];
					snprintf(msg, sizeof(msg), "%s\n", write_buf);
					ssize_t w = write(fifo_fd, msg, strlen(msg));
					if (w < 0) {
						perror("Ошибка записи в FIFO");
					}

					write_len = 0;
					fprintf(stderr, "\n%c ", prompt_in);
					fflush(stderr);
				}
				else if (write_len < BUFFER_SIZE - 1) {
					write_buf[write_len++] = c;
					fprintf(stderr, "%c", c);
					fflush(stderr);
				}
			}
			else {
				break;
			}
		}

		if (FD_ISSET(fifo_fd, &read_fds)) {
			int bytes_read = read(fifo_fd, read_buf, sizeof(read_buf) - 1);
			if (bytes_read > 0) {
				read_buf[bytes_read] = '\0';

				fprintf(stderr, "\n%c %s", prompt_out, read_buf);

				fprintf(stderr, "%c ", prompt_in);
				fflush(stderr);
			}
			else if (bytes_read == 0) {
				fprintf(stderr, "\nСобеседник вышел из чата.\n");
				break;
			}
		}
	}

	close(fifo_fd);
	return 0;
}
