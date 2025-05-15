#include "process.h"
#define __USE_GNU
#include <signal.h>
#include <stdio.h>
#include <alloca.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <sys/wait.h>

Pid process_new(ProcessFunc func, void *arg, ProcessConfig *cfg) {
    void *stack = alloca(cfg->stack_size);
    int flags = CLONE_NEWCGROUP | CLONE_NEWIPC | CLONE_NEWNET | CLONE_NEWNS |
            CLONE_NEWPID | CLONE_NEWUSER | CLONE_NEWUTS | SIGCHLD;
    Pid pid = clone(func, stack, flags, arg);
    if (pid < 0) {
        printf("failed to run new process: %s\n", strerror(errno));
    }
    return pid;
}

ExitStatus process_wait(Pid pid) {
    int status = -1;
    if (waitpid(pid, &status, 0) < 0) {
        printf("failed to wait child: %s\n", strerror(errno));
        return -1;
    }
    return status;
}
