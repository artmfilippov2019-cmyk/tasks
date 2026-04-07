#ifndef TASK4_OPTIONS_HELPERS_H
#define TASK4_OPTIONS_HELPERS_H

#define ELBRUS_VAL 256

int elbrus_valid(const char *value);
void print_incorrect(const char *form);
int parse_long_bad_argv(char *argv[]);

#endif
