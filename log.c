#include <stdio.h>
#include <stdarg.h>

#include "constants.h"

int log_level = ERROR;

void my_log(int level, char * format, ...) {
    if (level > log_level)
        return;
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}

int  set_log_level (int level) {
    int old_log_level = log_level;
    log_level = level;
    return old_log_level;
}
