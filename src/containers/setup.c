#include "setup.h"
#include "core/errlog.h"
#include "sys/fs.h"
#include "isoenv/proto.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>

static bool container_cmd_uid(Pid pid, IsoEnvProtoCmd *cmd) {
    char path[PATH_MAX];
    sprintf(path, "/proc/%d/uid_map", pid);
    if (!file_write(path, "%d 1000 1\n", cmd->uid)) return false;
    sprintf(path, "/proc/%d/gid_map", pid);
    if (!file_write(path, "%d 1000 1\n", cmd->uid)) return false;
    return true;
}

static bool container_cmd_net(Pid pid, IsoEnvProtoCmd *cmd) {
    char command[32];
    snprintf(command, 32, "./config/net.sh %d", pid);
    if (log_err(system(command) != 0, "failed to setup net")) return false;
    return true;
}

bool container_external_setup(ContainerExternalHandle *handle) {
    if (!isoenv_proto_handle(handle->conn, handle->pid, ISO_ENV_PROTO_CMD_UID, container_cmd_uid)) return false;
    if (!isoenv_proto_handle(handle->conn, handle->pid, ISO_ENV_PROTO_CMD_NET, container_cmd_net)) return false;
    printf("user setup completed successfully!\n");
    return true;
}
