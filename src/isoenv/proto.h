#pragma once

#include "sys/ipc.h"
#include "sys/process.h"
#include <stdbool.h>

typedef enum {
    ISO_ENV_PROTO_CMD_UID,
    ISO_ENV_PROTO_CMD_NET,
} IsoEnvProtoCmdKind;

typedef struct {
    IsoEnvProtoCmdKind kind;

    union {
        int uid;
    };
} IsoEnvProtoCmd;

typedef bool(*IsoEnvProtoCmdHandle)(Pid pid, IsoEnvProtoCmd *cmd);

bool isoenv_proto_handle(UnixSocket src, Pid pid, IsoEnvProtoCmdKind kind, IsoEnvProtoCmdHandle handle);
bool isoenv_proto_cmd(UnixSocket dst, IsoEnvProtoCmd cmd);

static inline IsoEnvProtoCmd isoenv_proto_cmd_new_uid(int uid) {
    IsoEnvProtoCmd result = {
        .kind = ISO_ENV_PROTO_CMD_UID,
        .uid = uid,
    };
    return result;
}

static inline IsoEnvProtoCmd isoenv_proto_cmd_new_net() {
    IsoEnvProtoCmd result = {
        .kind = ISO_ENV_PROTO_CMD_NET,
    };
    return result;
}
