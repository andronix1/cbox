#include "fs.h"
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

bool file_write(const char *path, const char *fmt, ...) {
    FILE *file = fopen(path, "w");
    if (!file) {
        printf("failed to open the file: %s\n", strerror(errno));
        return false;
    }
    va_list list;
    va_start(list, fmt);
    if (vfprintf(file, fmt, list) < 0) {
        va_end(list);
        fclose(file);
        return false;
    }
    va_end(list);
    fclose(file);
    return true;
}
