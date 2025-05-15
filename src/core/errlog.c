#include "errlog.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

bool log_err(bool err, const char *message) {
    if (err) {
        printf("%s: %s\n", message, strerror(errno));
    }
    return err;
}
