#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p[2];
    char buf[1];
    pipe(p);
    
    int pid = fork();
    if (pid < 0) {
        printf("error\n");
        exit(0);
    } 
    else if (pid == 0) {
        read(p[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(p[1], "p", 1);
        close(p[0]);
        close(p[1]);
        exit(0);
    } 
    else {
        write(p[1], "p", 1);
        wait(0);
        read(p[0], buf, 1);
        printf("%d: received pong\n", getpid());
        close(p[0]);
        close(p[1]);
    }
    exit(0);
}