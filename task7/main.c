#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "copy_fork.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Ошибка: неверное количество аргументов\n");
		return 1;
	}

	int in_fd = open(argv[1], O_RDONLY);
	if (in_fd < 0) {
		fprintf(stderr, "Ошибка: не удалось открыть входной файл '%s': ", argv[1]);
		die("");
	}

	pid_t pid = fork();
	if (pid < 0) {
		die("Ошибка fork: не удалось создать дочерний процесс");
	}

	const char *out_path = (pid == 0) ? "child_copy" : "parent_copy";
	int out_fd = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd < 0) {
		fprintf(stderr, "Ошибка: не удалось создать/открыть выходной файл '%s': ", out_path);
		die("");
	}

	copy_via_pread(in_fd, out_fd);

	if (close(out_fd) < 0) {
		fprintf(stderr, "Ошибка: не удалось закрыть выходной файл '%s': ", out_path);
		die("");
	}

	if (pid == 0) {
		if (close(in_fd) < 0) {
			fprintf(stderr, "Ошибка: не удалось закрыть входной файл в дочернем процессе\n");
			_exit(1);
		}
		_exit(0);
	}

	int status = 0;
	if (waitpid(pid, &status, 0) < 0) {
		die("Ошибка waitpid: не удалось дождаться завершения дочернего процесса");
	}
	if (close(in_fd) < 0) {
		die("Ошибка: не удалось закрыть входной файл в родительском процессе");
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		fprintf(stderr, "Ошибка: дочерний процесс завершился некорректно\n");
		return 1;
	}

	print_file_labeled("Parent:", "parent_copy");
	print_file_labeled("Child:", "child_copy");

	return 0;
}
