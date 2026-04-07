#ifndef TASK8_SIG_MATMUL_H
#define TASK8_SIG_MATMUL_H

#define N 100

int install_sigint_handler(int use_sigaction);
void fill_matrices(int a[N][N], int b[N][N]);
void multiply_once(int a[N][N], int b[N][N], int c[N][N]);
int sleep_half_second(void);

#endif
