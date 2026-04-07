#ifndef TASK1_PARALLEL_SORT_H
#define TASK1_PARALLEL_SORT_H

#include <pthread.h>

typedef struct {
	int start_index;
	int end_index;
} ThreadArgs;

extern int *global_array;
extern int array_size;

int compare_ints(const void *a, const void *b);
void *thread_sort(void *arg);
int *merge_segments(int num_threads, int *segment_starts, int *segment_ends);

#endif
