#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// primes.c（非递归版）
int main() {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[1]);
        while (1) {
            int prime;
            if (read(p[0], &prime, sizeof(int)) == 0) {
                close(p[0]);
                exit(0);
            }
            printf("prime %d\n", prime);

            int q[2];
            pipe(q);

            if (fork() == 0) {
                close(q[1]);
                p[0] = q[0];
                continue;  // 重复 loop
            }

            close(q[0]);
            int n;
            while (read(p[0], &n, sizeof(int)) > 0) {
                if (n % prime != 0) {
                    write(q[1], &n, sizeof(int));
                }
            }
            close(p[0]);
            close(q[1]);
            wait(0);
            exit(0);
        }
    } else {
        close(p[0]);
        for (int i = 2; i <= 280; ++i) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
}
