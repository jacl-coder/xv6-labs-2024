#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    char buf[128];
    int p[2];
    pipe(p);

    int pid=fork();

    if(pid==0){
        close(p[0]);
        write(p[1], "ping", 4);
        printf("%d: received ping\n",getpid());
        close(p[1]);
    }else{
        close(p[1]);
        wait(0);
        read(p[0], buf, 4);
        printf("%d: received pong\n", getpid());
        close(p[0]);
    }
    exit(0);
}