#include "line_sort.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int str_icmp(const char *a, const char *b) {
	for (;;) {
		unsigned char ca = (unsigned char)*a++;
		unsigned char cb = (unsigned char)*b++;
		if (ca == '\0' && cb == '\0') {
			return 0;
		}
		if (ca == '\0') {
			return -1;
		}
		if (cb == '\0') {
			return 1;
		}
		int da = tolower((int)ca);
		int db = tolower((int)cb);
		if (da != db) {
			return (da < db) ? -1 : 1;
		}
	}
}

static int line_has_visible(const char *s) {
	for (; *s; ++s) {
		if (isgraph((unsigned char)*s)) {
			return 1;
		}
	}
	return 0;
}

static int cmp_plain(const void *a, const void *b) {
	char *const *sa = a;
	char *const *sb = b;
	return strcmp(*sa, *sb);
}

static int cmp_lex(const void *a, const void *b) {
	char *const *sa = a;
	char *const *sb = b;
	return str_icmp(*sa, *sb);
}

static int cmp_rplain(const void *a, const void *b) {
	return -cmp_plain(a, b);
}

static int cmp_rlex(const void *a, const void *b) {
	return -cmp_lex(a, b);
}

sort_mode_t parse_mode(const char *s) {
	if (strcmp(s, "plain") == 0) {
		return SORT_PLAIN;
	}
	if (strcmp(s, "lex") == 0) {
		return SORT_LEX;
	}
	if (strcmp(s, "rplain") == 0) {
		return SORT_RPLAIN;
	}
	if (strcmp(s, "rlex") == 0) {
		return SORT_RLEX;
	}
	fprintf(stderr, "Ошибка: неизвестный режим сортировки '%s'\n", s);
	exit(1);
}

char *read_entire_file(const char *path, size_t *out_len) {
	FILE *f = fopen(path, "rb");
	if (!f) {
		perror(path);
		exit(1);
	}
	if (fseek(f, 0, SEEK_END) != 0) {
		fclose(f);
		fprintf(stderr, "Ошибка: не удалось переместиться в конец файла '%s'\n", path);
		exit(1);
	}
	long sz = ftell(f);
	if (sz < 0) {
		fclose(f);
		fprintf(stderr, "Ошибка: не удалось определить размер файла '%s'\n", path);
		exit(1);
	}
	if (fseek(f, 0, SEEK_SET) != 0) {
		fclose(f);
		fprintf(stderr, "Ошибка: не удалось вернуться в начало файла '%s'\n", path);
		exit(1);
	}
	char *buf = malloc((size_t)sz + 1u);
	if (!buf) {
		fclose(f);
		fprintf(stderr, "Ошибка выделения памяти\n");
		exit(1);
	}
	size_t n = fread(buf, 1, (size_t)sz, f);
	fclose(f);
	if (n != (size_t)sz) {
		free(buf);
		fprintf(stderr, "Ошибка: не удалось прочитать файл '%s'\n", path);
		exit(1);
	}
	buf[sz] = '\0';
	*out_len = (size_t)sz;
	return buf;
}

void collect_lines(char *buf, char ***lines_out, size_t *count_out) {
	size_t cap = 16;
	size_t n = 0;
	char **lines = malloc(cap * sizeof *lines);
	if (!lines) {
		fprintf(stderr, "Ошибка выделения памяти\n");
		exit(1);
	}

	char *p = buf;
	while (*p) {
		char *start = p;
		while (*p && *p != '\n') {
			++p;
		}
		char *end = p;
		if (*p == '\n') {
			*p++ = '\0';
		}
		if (end > start && end[-1] == '\r') {
			end[-1] = '\0';
		}
		if (line_has_visible(start)) {
			if (n == cap) {
				cap *= 2;
				char **nl = realloc(lines, cap * sizeof *nl);
				if (!nl) {
					free(lines);
					fprintf(stderr, "Ошибка выделения памяти\n");
					exit(1);
				}
				lines = nl;
			}
			lines[n++] = start;
		}
	}

	*lines_out = lines;
	*count_out = n;
}

int (*line_sort_comparator(sort_mode_t mode))(const void *, const void *) {
	switch (mode) {
	case SORT_PLAIN:
		return cmp_plain;
	case SORT_LEX:
		return cmp_lex;
	case SORT_RPLAIN:
		return cmp_rplain;
	case SORT_RLEX:
		return cmp_rlex;
	default:
		return cmp_plain;
	}
}
