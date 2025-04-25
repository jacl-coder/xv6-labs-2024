#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(2, "Usage: sleep seconds\n");
        exit(1);
    }
    int seconds = atoi(argv[1]);
    if(seconds < 0) {
        fprintf(2, "sleep: seconds must be a non-negative integer\n");
        exit(1);
    }
    if(sleep(seconds) < 0) {
        fprintf(2, "sleep: failed to sleep for %d seconds\n", seconds);
        exit(1);
    }
    exit(0);
}