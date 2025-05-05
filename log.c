#include <stdio.h>
#include <stdarg.h>

#include "constants.h"
#include "command.h"

int log_level = ERROR;

extern word reg[REGSIZE];

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

void reg_dump() {
    my_log(TRACE, "\n");
    my_log(TRACE, flags & Npos ? "N" : "-");
    my_log(TRACE, flags & Zpos ? "Z" : "-");
    my_log(TRACE, flags & Vpos ? "V" : "-");
    my_log(TRACE, flags & Cpos ? "C" : "-");
    my_log(TRACE, "\n");
    for (int i = 0; i < REGSIZE; i++) {
        my_log(TRACE, "r%d:%o ", i, reg[i]);
    }
    my_log(TRACE, "\n");
}
