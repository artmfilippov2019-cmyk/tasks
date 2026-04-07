#define _POSIX_C_SOURCE 200809L

#include "sig_matmul.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static volatile sig_atomic_t g_i;
static volatile sig_atomic_t g_j;
static volatile sig_atomic_t g_k;

static void print_indices_1based(void) {
	printf("%d %d\n", (int)g_i + 1, (int)g_k + 1);
	printf("%d %d\n", (int)g_k + 1, (int)g_j + 1);
	printf("%d %d\n", (int)g_i + 1, (int)g_j + 1);
	fflush(stdout);
}

static void on_sigint_signal(int signo) {
	(void)signo;
	print_indices_1based();
	signal(SIGINT, SIG_DFL);
}

static void on_sigint_sigaction(int signo) {
	(void)signo;
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	print_indices_1based();
	if (sigaction(SIGINT, &sa, NULL) != 0) {
		_Exit(1);
	}
}

int sleep_half_second(void) {
	struct timespec req = {.tv_sec = 0, .tv_nsec = 500000000L};
	struct timespec rem;
	while (nanosleep(&req, &rem) == -1) {
		if (errno != EINTR) {
			return -1;
		}
		req = rem;
	}
	return 0;
}

void fill_matrices(int a[N][N], int b[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			a[i][j] = (i + j) % 7 + 1;
			b[i][j] = (i * 3 + j * 2) % 11 + 1;
		}
	}
}

void multiply_once(int a[N][N], int b[N][N], int c[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				g_i = (sig_atomic_t)i;
				g_j = (sig_atomic_t)j;
				g_k = (sig_atomic_t)k;
				c[i][j] += a[i][k] * b[k][j];

				sleep_half_second();
			}
		}
	}
}

int install_sigint_handler(int use_sigaction) {
	g_i = g_j = g_k = 0;

	if (use_sigaction) {
		struct sigaction sa;
		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = on_sigint_sigaction;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		if (sigaction(SIGINT, &sa, NULL) != 0) {
			perror("sigaction");
			return -1;
		}
	} 
	else {
		if (signal(SIGINT, on_sigint_signal) == SIG_ERR) {
			perror("signal");
			return -1;
		}
	}
	return 0;
}
