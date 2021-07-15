#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "parse_syscall.h"

extern int errno;

#define ARCH_TYPE_64 64
#define ARCH_TYPE_32 32

// #define ARCH_ORIG = ORIG_RAX

// #ifndef __x86_64__

// #define ARCH_ORIG = ORIG_EAX

// #endif

void from_pid(int pid)
{
    printf("Executing from pid: %d\n", pid);
}

int exec_child(int argc, char *argv[])
{
    char *arguments[argc + 1];
    if (NULL == memcpy(arguments, argv, argc * sizeof(char *)))
    {
        errx(EXIT_FAILURE,
             "Error while using memcpy to copy the argument list.");
    }
    arguments[argc] = NULL;
    int errnum = 0;

    /*
     * From the ptrace(2) manpage:
     *  > Indicate  that this process is to be traced by its parent.
     *  > A process probably shouldn't make this request if its
     *  > parent isn't expecting to trace it.
     */

    long rt = ptrace(PTRACE_TRACEME);
    if (rt == -1)
    {
        errnum = errno;
        perror("error while requesting a TRACEME request.");
        errx(EXIT_FAILURE,
             "Error while sending the request PTRACE_TRACEME to the child %d: "
             "%s",
             getpid(), strerror(errnum));
    }
    /*
     * Send a SIGSTOP signal to itself (i.e. the child itself, which will be
     * catched by the parent's child to perform the tracing).
     */

    if (kill(getpid(), SIGSTOP) == -1)
    {
        errnum = errno;
        perror("error while sending SIGSTOP signal to the child.");
        errx(EXIT_FAILURE, "Error while sending SIGSTOP to the child %d: %s",
             getpid(), strerror(errnum));
    }
    return execvp(arguments[0], arguments);
}

int catch_syscall(int child)
{
    int status = 0;
    while (1)
    {
        int res = ptrace(PTRACE_SYSCALL, child, 0, 0);
        if (res == -1 || waitpid(child, &status, 0) == -1
            || (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80))
        {
            return 0;
        }
        if (WIFEXITED(status))
            return 1;
    }
}

int get_trace(int child)
{
    int status;
    long syscall_id;
    long res;

    /* wait linked to the SIGSTOP signal sent to the child above. */
    int errnum = 0;
    if (waitpid(child, &status, 0) == -1)
    {
        errnum = errno;
        perror("error while waiting for the child process");
        errx(EXIT_FAILURE, "Error while waiting for pid %d: %s", child,
             strerror(errnum));
    }

    /*
     * PTRACE_SETOPTIONS allows to put the PTRACE_O_TRACESYSGOOD option
     *   to only send a SIGTRAP when a syscall related call is done
     */
    long rt = ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

    if (rt == -1)
    {
        errnum = errno;
        perror("error while tracing the child.");
        errx(EXIT_FAILURE, "Error while tracing the child %d: %s", child,
             strerror(errnum));
    }

    struct hash_map *p = parse_syscall(ARCH_TYPE_64);
    if (NULL == p)
    {
        perror("parse_syscall");
        return EXIT_FAILURE;
    }

    while (1)
    {
        if (catch_syscall(child) != 0)
        {
            // end of the child process
            break;
        }
        /* the child now enter in a syscall.
         * We look into RAX because it is where syscall values are stored.
         * On different architecture, we would potentially look elsewhere
         */
        syscall_id = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_RAX);
        if (syscall_id == -1)
        {
            errnum = errno;
            perror("error while tracing the child.");
            errx(EXIT_FAILURE, "Error while tracing the child %d: %s", child,
                 strerror(errnum));
        }

        char str[15];
        sprintf(str, "%ld", syscall_id);

        fprintf(stdout, "%s ", hash_map_get(p, str));

        if (catch_syscall(child) != 0)
        {
            // end of the child process.
            break;
        }

        /*
         * End of the syscall, we can get its return value which is located in
         * the RAX register (EAX in 32-bits architecture).
         */
        res = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * RAX);
        fprintf(stdout, "has a return code of: %ld\n", res);
    }
    hash_map_free(p);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        errx(EXIT_FAILURE, "usage: ./my_strace elf/command");
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
