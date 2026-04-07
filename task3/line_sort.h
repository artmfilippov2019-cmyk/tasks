#ifndef TASK3_LINE_SORT_H
#define TASK3_LINE_SORT_H

#include <stddef.h>

typedef enum {
	SORT_PLAIN,
	SORT_LEX,
	SORT_RPLAIN,
	SORT_RLEX
} sort_mode_t;

sort_mode_t parse_mode(const char *s);
char *read_entire_file(const char *path, size_t *out_len);
void collect_lines(char *buf, char ***lines_out, size_t *count_out);
int (*line_sort_comparator(sort_mode_t mode))(const void *, const void *);

#endif
