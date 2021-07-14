#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <unistd.h>

extern int errno;


void from_pid(int pid)
{
    printf("Executing from pid: %d\n", pid);
}

int exec_child(int argc, char *argv[])
{
    if (argc == 0 || NULL == argv)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int get_trace(pid_t child)
{
    if (child == -1)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        errx(EXIT_FAILURE, "usage: ./my_strace {list of pid}");
    }
    printf("============== Syscall Trace of %s ==============\n", argv[1]);
    int errnum = 0;

    int pid = fork();
    if (pid == -1)
    {
        errnum = errno;
        perror("error while forking");
        errx(EXIT_FAILURE, "Error while forking: %s", strerror(errnum));
    }
    if (pid == 0)
    {
        return exec_child(argc - 1, argv + 1); 
    }

    return get_trace(pid);
}
