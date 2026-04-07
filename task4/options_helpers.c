#define _GNU_SOURCE
#include "options_helpers.h"
#include <getopt.h>
#include <stdio.h>
#include <string.h>

static const char *const elbrus_allowed[] = {
	"1c+", "2c+", "2c3", "4c", "8c", "16c", NULL,
};

int elbrus_valid(const char *value) {
	if (value == NULL) {
		return 0;
	}
	for (int i = 0; elbrus_allowed[i] != NULL; i++) {
		if (strcmp(value, elbrus_allowed[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

void print_incorrect(const char *form) {
	printf("Некорректная опция: '%s'\n", form);
}

int parse_long_bad_argv(char *argv[]) {
	char *arg = argv[optind - 1];
	if (strncmp(arg, "--", 2) != 0) {
		print_incorrect("?");
		return 1;
	}
	print_incorrect(arg + 2);
	return 1;
}
