#include "container.h"
#include "containers/setup.h"
#include "sys/ipc.h"
#include "isoenv/isoenv.h"
#include "sys/process.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int container_main(ContainerInternalHandle *handle) {
    void *arg = handle->run.arg;
    if (!isoenv_setup(handle->conn, &handle->config)) {
        printf("failed to setup isolated environment\n");
        free(handle);
        return 1;
    }
    int result = handle->run.main(handle->run.arg);
    return result;
}

bool container_run(ContainerConfig *cfg, ContainerRunConfig *run_cfg, ContainerExternalHandle *handle) {
    SocketPair sockets;
    if (!ipc_socket_pair_new(&sockets)) return false;

    ContainerInternalHandle *ihandle = malloc(sizeof(ContainerInternalHandle));
    ihandle->conn = sockets[0];
    memcpy(&ihandle->config, &cfg->isoenv, sizeof(IsoEnvConfig));
    memcpy(&ihandle->run, run_cfg, sizeof(ContainerRunConfig));

    Pid pid = process_new((ProcessFunc)container_main, ihandle, &cfg->process);
    if (process_new_failed(pid)) return false;

    printf("runnning container on pid: %d\n", pid);

    handle->name = cfg->name;
    handle->conn = sockets[1];
    handle->pid = pid;

    container_external_setup(handle);
    free(ihandle);
    return true;
}

int container_wait(ContainerExternalHandle *handle) {
    return process_wait(handle->pid);
}
