#define _POSIX_C_SOURCE 200809L

#include "longest_run.h"
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int grow(int **arr, size_t n, int *capacity) {
	if (n < (size_t)*capacity) {
		return 0;
	}
	int new_cap = *capacity * 2;
	int *tmp = (int *)realloc(*arr, (size_t)new_cap * sizeof *tmp);
	if (!tmp) {
		return -1;
	}
	*arr = tmp;
	*capacity = new_cap;
	return 0;
}

static int read_ints_tty_one_line(int **arr, size_t *n, int *capacity) {
	char *line = NULL;
	size_t linecap = 0;
	ssize_t len = getline(&line, &linecap, stdin);
	if (len < 0) {
		free(line);
		return 0;
	}
	for (char *p = line; *p != '\0';) {
		while (*p != '\0' && isspace((unsigned char)*p)) {
			p++;
		}
		if (*p == '\0') {
			break;
		}
		char *end = NULL;
		long v = strtol(p, &end, 10);
		if (end == p) {
			break;
		}
		if (v < (long)INT_MIN || v > (long)INT_MAX) {
			free(line);
			return -2;
		}
		if (grow(arr, *n, capacity) != 0) {
			free(line);
			return -1;
		}
		(*arr)[(*n)++] = (int)v;
		p = end;
	}
	free(line);
	return 0;
}

static int read_ints_until_eof(int **arr, size_t *n, int *capacity) {
	int val;
	while (scanf("%d", &val) == 1) {
		if (grow(arr, *n, capacity) != 0) {
			return -1;
		}
		(*arr)[(*n)++] = val;
	}
	return 0;
}

int longest_strictly_increasing_run(void) {
	int capacity = 1024;
	int *arr = (int *)malloc((size_t)capacity * sizeof *arr);
	if (!arr) {
		fprintf(stderr, "Ошибка выделения памяти\n");
		return EXIT_FAILURE;
	}

	size_t n = 0;
	if (isatty(fileno(stdin))) {
		int rc = read_ints_tty_one_line(&arr, &n, &capacity);
		if (rc != 0) {
			if (rc == -1) {
				fprintf(stderr, "Ошибка выделения памяти\n");
			}
			free(arr);
			return EXIT_FAILURE;
		}
	} 
	else {
		if (read_ints_until_eof(&arr, &n, &capacity) != 0) {
			fprintf(stderr, "Ошибка: ошибка выделения памяти\n");
			free(arr);
			return EXIT_FAILURE;
		}
	}

	if (n == 0) {
		free(arr);
		return 0;
	}

	size_t best_start = 0;
	size_t best_len = 1;
	long long best_sum = arr[0];

	size_t run_start = 0;
	long long run_sum = arr[0];
	size_t run_len = 1;

	for (size_t i = 1; i < n; i++) {
		if (arr[i] > arr[i - 1]) {
			run_sum += arr[i];
			run_len++;
		}
		else {
			if (run_len > best_len || (run_len == best_len && run_sum > best_sum)) {
				best_len = run_len;
				best_sum = run_sum;
				best_start = run_start;
			}
			run_start = i;
			run_sum = arr[i];
			run_len = 1;
		}
	}
	if (run_len > best_len || (run_len == best_len && run_sum > best_sum)) {
		best_len = run_len;
		best_sum = run_sum;
		best_start = run_start;
	}

	for (size_t i = 0; i < best_len; i++) {
		if (i) {
			putchar(' ');
		}
		printf("%d", arr[best_start + i]);
	}
	putchar('\n');

	free(arr);
	return 0;
}
