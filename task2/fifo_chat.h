#ifndef TASK2_FIFO_CHAT_H
#define TASK2_FIFO_CHAT_H

void signal_handler(int sig);

extern int fifo_fd;
extern char *fifo_path;

#endif
