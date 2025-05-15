#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef int(*ProcessFunc)(void *arg);
typedef uint32_t Pid;
typedef int ExitStatus;

typedef struct {
    size_t stack_size;
} ProcessConfig;

Pid process_new(ProcessFunc func, void *arg, ProcessConfig *cfg);
ExitStatus process_wait(Pid pid);
static inline bool process_new_failed(Pid pid) { return pid < 0; }
