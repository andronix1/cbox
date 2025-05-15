#include "containers/container.h"
#include "core/attr.h"
#include "core/errlog.h"
#include <stdio.h>
#include <unistd.h>

int main_child(void *arg UNUSED) {
    char *args[] = { "sh", NULL };
    printf("wellcome to shell!\n");
    if (log_err(execv("/bin/sh", args) < 0, "failed to run shell")) return 1;
    return 0;
}

int main() {
    if (getuid() != 0) {
        printf("must be executed from root\n");
        return 1;
    }
    ContainerConfig cfg = {
        .name = "test",
        .process = {
            .stack_size = 1024 * 1024
        },
        .isoenv = {
            .rootfs = "../runtime/test",
            .hostname = "runner",
        },
    };
    ContainerRunConfig run_cfg = {
        .main = main_child,
        .arg = NULL
    };
    ContainerExternalHandle container;
    if (!container_run(&cfg, &run_cfg, &container)) return 1;
    container_wait(&container);
    return 0;
}
