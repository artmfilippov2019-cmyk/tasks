#include <stdio.h>
#include <stdlib.h>
#include "line_sort.h"

int main(int argc, char **argv) {
	if (argc != 4) {
		fprintf(stderr, "Ошибка: неверное количество аргументов\n");
		return 1;
	}

	sort_mode_t mode = parse_mode(argv[3]);

	size_t file_len;
	char *buf = read_entire_file(argv[1], &file_len);
	(void)file_len;

	char **lines;
	size_t nlines;
	collect_lines(buf, &lines, &nlines);

	int (*cmp)(const void *, const void *) = line_sort_comparator(mode);
	qsort(lines, nlines, sizeof *lines, cmp);

	FILE *out = fopen(argv[2], "wb");
	if (!out) {
		perror(argv[2]);
		free(lines);
		free(buf);
		return 1;
	}
	for (size_t i = 0; i < nlines; ++i) {
		fputs(lines[i], out);
		fputc('\n', out);
	}
	fclose(out);

	free(lines);
	free(buf);
	return 0;
}
