#ifndef TASK7_COPY_FORK_H
#define TASK7_COPY_FORK_H

void die(const char *msg);
void copy_via_pread(int in_fd, int out_fd);
void print_file_labeled(const char *label_line, const char *path);

#endif
