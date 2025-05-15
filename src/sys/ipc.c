#include "ipc.h"
#include "sys/socket.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

bool ipc_socket_pair_new(SocketPair *output) {
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, &(*output)[0]) < 0) {
        printf("failed to create socket pair: %s\n", strerror(errno));
        return false;
    }
    return true;
}
