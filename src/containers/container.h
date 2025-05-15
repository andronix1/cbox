#pragma once

#include "sys/ipc.h"
#include "sys/process.h"
#include "isoenv/isoenv.h"

typedef struct {
    int (*main)(void*);
    void *arg;
} ContainerRunConfig;

typedef struct {
    IsoEnvConfig config;
    ContainerRunConfig run;
    UnixSocket conn;
} ContainerInternalHandle;

typedef struct {
    const char *name;
    Pid pid;
    UnixSocket conn;
} ContainerExternalHandle;

typedef struct {
    const char *name;
    IsoEnvConfig isoenv;
    ProcessConfig process;
} ContainerConfig;

bool container_run(ContainerConfig *cfg, ContainerRunConfig *run_cfg, ContainerExternalHandle *handle);
int container_wait(ContainerExternalHandle *handle);
