#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 280
#define FIRST_PRIME 2

void primes(int in_fd) __attribute__((noreturn));

void primes(int in_fd)
{
    int prime;
    if (read(in_fd, &prime, sizeof(int)) == 0)
    {
        close(in_fd);
        exit(0);
    }

    printf("prime %d\n", prime);

    int out_pipe[2];
    pipe(out_pipe);

    if (fork() == 0)
    {
        close(out_pipe[0]);
        int num;
        while (read(in_fd, &num, sizeof(int)))
        {
            if (num % prime != 0)
            {
                write(out_pipe[1], &num, sizeof(int));
            }
        }
        close(in_fd);
        close(out_pipe[1]);
        exit(0);
    }
    else
    {
        close(out_pipe[1]);
        close(in_fd);
        primes(out_pipe[0]);
    }
}

int main(int argc, char *argv[])
{
    int out_pipe[2];
    pipe(out_pipe);

    if (fork() == 0)
    {
        close(out_pipe[0]);
        for (int i = FIRST_PRIME; i <= MAX; i++)
        {
            write(out_pipe[1], &i, sizeof(int));
        }
        close(out_pipe[1]);
        exit(0);
    }
    else
    {
        close(out_pipe[1]);
        primes(out_pipe[0]);
    }

    exit(0);
}