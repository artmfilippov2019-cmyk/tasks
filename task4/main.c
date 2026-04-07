#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "options_helpers.h"

int main(int argc, char *argv[]) {
	static struct option long_opts[] = {
		{"elbrus", required_argument, NULL, ELBRUS_VAL},
		{NULL, 0, NULL, 0},
	};

	char *short_order = (char *)malloc((size_t)argc);
	char **long_parts = (char **)malloc((size_t)argc * sizeof(char *));
	if (short_order == NULL || long_parts == NULL) {
		free(short_order);
		free(long_parts);
		return 1;
	}
	int n_short = 0;
	int n_long = 0;

	int long_index = 0;
	opterr = 0;

	int c;
	while ((c = getopt_long(argc, argv, "mcst", long_opts, &long_index)) != -1) {
		switch (c) {
		case 'm':
		case 'c':
		case 's':
		case 't':
			short_order[n_short++] = (char)c;
			break;
		case ELBRUS_VAL:
			if (!elbrus_valid(optarg)) {
				char buf[256];
				const char *v = optarg != NULL ? optarg : "";
				snprintf(buf, sizeof(buf), "elbrus=%s", v);
				print_incorrect(buf);
				free(short_order);
				for (int i = 0; i < n_long; i++) {
					free(long_parts[i]);
				}
				free(long_parts);
				return 0;
			}
			size_t len = strlen("elbrus=") + strlen(optarg) + 1;
			char *s = (char *)malloc(len);
			if (s == NULL) {
				free(short_order);
				for (int i = 0; i < n_long; i++) {
				   free(long_parts[i]);
				}
				free(long_parts);
				return 1;
			}
			snprintf(s, len, "elbrus=%s", optarg);
			long_parts[n_long++] = s;
			break;
		case '?':
			if (optopt != 0) {
				char buf[8];
				snprintf(buf, sizeof(buf), "%c", optopt);
				print_incorrect(buf);
			} 
			else {
				parse_long_bad_argv(argv);
			}
			free(short_order);
			for (int i = 0; i < n_long; i++) {
				free(long_parts[i]);
			}
			free(long_parts);
			return 0;
		default:
			free(short_order);
			for (int i = 0; i < n_long; i++) {
				free(long_parts[i]);
			}
			free(long_parts);
			return 1;
		}
	}

	printf("Short options:");
	for (int i = 0; i < n_short; i++) {
		printf(" '%c'", short_order[i]);
	}
	printf("\n");

	printf("Long options:");
	for (int i = 0; i < n_long; i++) {
		printf(" '%s'", long_parts[i]);
	}
	printf("\n");

	printf("Non options:");
	for (int i = optind; i < argc; i++) {
		printf(" '%s'", argv[i]);
	}
	printf("\n");

	free(short_order);
	for (int i = 0; i < n_long; i++) {
		free(long_parts[i]);
	}
	free(long_parts);
	return 0;
}
