#include "fifo_chat.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fifo_fd = -1;
char *fifo_path = NULL;

void signal_handler(int sig) {
	(void)sig;
	if (fifo_fd != -1) {
		close(fifo_fd);
	}
	printf("\nЧат завершен.\n");
	exit(0);
}
