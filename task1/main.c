#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "parallel_sort.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Ошибка: неверное количество аргументов\n");
		return EXIT_FAILURE;
	}

	int num_threads = atoi(argv[1]);
	if (num_threads < 1) {
		fprintf(stderr, "Ошибка: количество потоков должно быть положительным целым числом.\n");
		return EXIT_FAILURE;
	}

	int capacity = 1024;
	global_array = (int *)malloc((size_t)capacity * sizeof(int));
	if (!global_array) {
		perror("Ошибка выделения памяти");
		return EXIT_FAILURE;
	}

    int val;
	while (scanf("%d", &val) == 1) {
		if (array_size >= capacity) {
			capacity *= 2;
			int *temp = (int *)realloc(global_array, (size_t)capacity * sizeof(int));
			if (!temp) {
				perror("Ошибка выделения памяти");
				free(global_array);
				return EXIT_FAILURE;
			}
			global_array = temp;
		}
		global_array[array_size++] = val;
	}

	if (array_size == 0) {
		free(global_array);
		return EXIT_SUCCESS;
	}

	if (num_threads > array_size) {
		num_threads = array_size;
	}

	pthread_t *threads = (pthread_t *)malloc((size_t)num_threads * sizeof(pthread_t));
	int *segment_starts = (int *)malloc((size_t)num_threads * sizeof(int));
	int *segment_ends = (int *)malloc((size_t)num_threads * sizeof(int));

	if (!threads || !segment_starts || !segment_ends) {
		perror("Ошибка выделения памяти");
		return EXIT_FAILURE;
	}

	int base_chunk_size = array_size / num_threads;
	int remainder = array_size % num_threads;
	int current_index = 0;

	for (int i = 0; i < num_threads; i++) {
		ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
		if (!args) {
			perror("Ошибка выделения памяти");
			return EXIT_FAILURE;
		}

		args->start_index = current_index;
		int chunk_size = base_chunk_size + (i < remainder ? 1 : 0);
		args->end_index = current_index + chunk_size - 1;

		segment_starts[i] = args->start_index;
		segment_ends[i] = args->end_index;

		current_index = args->end_index + 1;

		if (pthread_create(&threads[i], NULL, thread_sort, (void *)args) != 0) {
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	int *sorted_array = merge_segments(num_threads, segment_starts, segment_ends);

	for (int i = 0; i < array_size; i++) {
		printf("%d%c", sorted_array[i], (i == array_size - 1) ? '\n' : ' ');
	}

	free(sorted_array);
	free(threads);
	free(segment_starts);
	free(segment_ends);
	free(global_array);

	return EXIT_SUCCESS;
}
