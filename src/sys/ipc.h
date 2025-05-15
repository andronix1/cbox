#pragma once

#include <stdbool.h>
typedef int UnixSocket;

typedef int SocketPair[2];

bool ipc_socket_pair_new(SocketPair *output);
