#define _POSIX_C_SOURCE 200809L

#include "copy_fork.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUF_SIZE 4096

void die(const char *msg) {
	perror(msg);
	exit(1);
}

void copy_via_pread(int in_fd, int out_fd) {
	struct stat st;
	if (fstat(in_fd, &st) < 0) {
		die("Ошибка fstat: не удалось получить информацию о файле");
	}

	char buf[BUF_SIZE];
	off_t off = 0;
	while (off < st.st_size) {
		size_t chunk = (size_t)(st.st_size - off);
		if (chunk > BUF_SIZE) {
			chunk = BUF_SIZE;
		}
		ssize_t n = pread(in_fd, buf, chunk, off);
		if (n < 0) {
			die("Ошибка pread: не удалось прочитать данные из файла");
		}
		if (n == 0) {
			break;
		}

		char *p = buf;
		ssize_t left = n;
		while (left > 0) {
			ssize_t w = write(out_fd, p, (size_t)left);
			if (w < 0) {
				die("Ошибка write: не удалось записать данные в файл");
			}
			p += w;
			left -= w;
		}
		off += n;
	}
}

void print_file_labeled(const char *label_line, const char *path) {
	printf("%s\n", label_line);
	fflush(stdout);

	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Ошибка: не удалось открыть файл для печати '%s': ", path);
		die("");
	}

	char buf[BUF_SIZE];
	for (;;) {
		ssize_t n = read(fd, buf, sizeof buf);
		if (n < 0) {
			die("Ошибка read: не удалось прочитать файл для печати");
		}
		if (n == 0) {
			break;
		}
		char *p = buf;
		ssize_t left = n;
		while (left > 0) {
			ssize_t w = write(STDOUT_FILENO, p, (size_t)left);
			if (w < 0) {
				die("Ошибка write: не удалось вывести данные на экран");
			}
			p += w;
			left -= w;
		}
	}

	if (close(fd) < 0) {
		die("Ошибка close: не удалось закрыть файл после печати");
	}

	putchar('\n');
	fflush(stdout);
}
