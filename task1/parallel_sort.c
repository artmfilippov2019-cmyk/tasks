#include "parallel_sort.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int *global_array = NULL;
int array_size = 0;

int compare_ints(const void *a, const void *b) {
	int arg1 = *(const int *)a;
	int arg2 = *(const int *)b;
	if (arg1 < arg2) {
		return -1;
	}
	if (arg1 > arg2) {
		return 1;
	}
	return 0;
}

void *thread_sort(void *arg) {
	ThreadArgs *args = (ThreadArgs *)arg;

	int count = args->end_index - args->start_index + 1;
	if (count > 0) {
		qsort(global_array + args->start_index, (size_t)count, sizeof(int), compare_ints);
	}

	free(args);
	return NULL;
}

int *merge_segments(int num_threads, int *segment_starts, int *segment_ends) {
	int *result = (int *)malloc((size_t)array_size * sizeof(int));
	if (!result) {
		perror("Ошибка выделения памяти");
		exit(EXIT_FAILURE);
	}

	int *current_indices = (int *)calloc((size_t)num_threads, sizeof(int));
	if (!current_indices) {
		perror("Ошибка выделения памяти");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < array_size; i++) {
		int min_val = INT_MAX;
		int min_thread = -1;

		for (int t = 0; t < num_threads; t++) {
			int start = segment_starts[t];
			int end = segment_ends[t];
			int current_idx = start + current_indices[t];

			if (current_idx <= end) {
				int val = global_array[current_idx];
				if (val < min_val) {
					min_val = val;
					min_thread = t;
				}
			}
		}

		if (min_thread != -1) {
			result[i] = min_val;
			current_indices[min_thread]++;
		}
	}

	free(current_indices);
	return result;
}
