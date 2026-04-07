#include <stdio.h>
#include <string.h>
#include "sig_matmul.h"

int main(int argc, char *argv[]) {
	int use_sigaction = -1;

	if (argc != 2) {
		fprintf(stderr, "Ошибка: неверное количество аргументов.\n");
		return 1;
	}
	if (strcmp(argv[1], "--signal") == 0) {
		use_sigaction = 0;
	} 
	else if (strcmp(argv[1], "--sigaction") == 0) {
		use_sigaction = 1;
	}
	else {
		return 1;
	}

	if (install_sigint_handler(use_sigaction) != 0) {
		return 1;
	}

	int a[N][N];
	int b[N][N];
	int c[N][N];
	fill_matrices(a, b);

	while (1) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				c[i][j] = 0;
			}
		}
		multiply_once(a, b, c);
		if (sleep_half_second() != 0) {
			fprintf(stderr, "Ошибка: nanosleep - не удалось приостановить выполнение\n");
			return 1;
		}
	}
}
