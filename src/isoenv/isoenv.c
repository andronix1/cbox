#include "isoenv.h"
#include "core/errlog.h"
#include "sys/ipc.h"
#include "isoenv/proto.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#define __USE_GNU
#include <sched.h>

static inline bool isoenv_setup_root(const char *rootfs) {
    printf("changing root to %s...\n", rootfs);
    if (log_err(chroot(rootfs) < 0, "failed to chroot")) return false;
    if (log_err(chdir("/") < 0, "failed to change dir to root")) return false;
    return true;
}

static inline bool isoenv_setup_hostname(const char *hostname) {
    printf("changing hostname...\n");
    if (log_err(sethostname(hostname, strlen(hostname)) < 0, "failed to set hostname")) return false;
    return true;
}

static inline bool isoenv_setup_env() {
    printf("changing env...\n");
    if (log_err(clearenv() < 0, "failed to clear encironment")) return false;
    if (log_err(setenv("PATH", "/bin:/sbin:/usr/bin:/usr/sbin", true) < 0, "failed to set PATH")) return false;
    return true;
}

static inline bool isoenv_setup_user(UnixSocket conn) {
    printf("changing user...\n");
    if (!isoenv_proto_cmd(conn, isoenv_proto_cmd_new_uid(0))) return false;
    if (log_err(setuid(0), "failed to change uid")) return false;
    if (log_err(setgid(0), "failed to change gid")) return false;
    gid_t gid = 0;
    if (log_err(setgroups(1, &gid), "failed to change gid")) return false;
    return true;
}

static inline bool isoenv_setup_net(UnixSocket conn) {
    printf("connecting to network...\n");
    if (!isoenv_proto_cmd(conn, isoenv_proto_cmd_new_net())) return false;
    return true;
}

bool isoenv_setup(UnixSocket conn, IsoEnvConfig *cfg) {
    if (!isoenv_setup_root(cfg->rootfs)) return false;
    if (!isoenv_setup_hostname(cfg->hostname)) return false;
    if (!isoenv_setup_env()) return false;
    if (!isoenv_setup_user(conn)) return false;
    if (!isoenv_setup_net(conn)) return false;
    return true;
}
