#pragma once

#include "sys/ipc.h"
#include "sys/process.h"
#include <stdbool.h>

typedef struct {
    const char *rootfs;
    const char *hostname;
} IsoEnvConfig;

typedef struct {
    IsoEnvConfig config;
    UnixSocket conn;
} IsoEnvEnvironment;

bool isoenv_setup(UnixSocket conn, IsoEnvConfig *cfg);
bool isoenv_external_setup(UnixSocket conn, Pid pid);
