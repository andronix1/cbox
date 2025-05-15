#include "proto.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>

static inline bool isoenv_proto_cmd_begin(UnixSocket src, IsoEnvProtoCmd *dst) {
    if (recv(src, dst, sizeof(IsoEnvProtoCmd), 0) < 0) {
        printf("failed to receive isoenv cmd: %s\n", strerror(errno));
        return false;
    }
    return true;
}

static inline bool isoenv_proto_cmd_end(UnixSocket src, bool status) {
    if (send(src, &status, sizeof(status), 0) < 0) {
        printf("failed to send status: %s\n", strerror(errno));
        return false;
    }
    return true;
}

bool isoenv_proto_handle(UnixSocket src, Pid pid, IsoEnvProtoCmdKind kind, IsoEnvProtoCmdHandle handle) {
    IsoEnvProtoCmd cmd;
    if (!isoenv_proto_cmd_begin(src, &cmd)) {
        return false;
    }
    assert(cmd.kind == kind);
    bool status = handle(pid, &cmd);
    if (!isoenv_proto_cmd_end(src, status)) {
        return false;
    }
    return status;
}

bool isoenv_proto_cmd(UnixSocket dst, IsoEnvProtoCmd cmd) {
    if (send(dst, &cmd, sizeof(IsoEnvProtoCmd), 0) < 0) {
        printf("failed to receive isoenv cmd: %s\n", strerror(errno));
        return false;
    }
    bool status;
    if (recv(dst, &status, sizeof(status), 0) < 0) {
        printf("failed to receive isoenv cmd: %s\n", strerror(errno));
        return false;
    }
    return status;
}
