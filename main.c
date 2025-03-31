#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "memory.h"
#include "log.h"
#include "command.h"

void run() {
    pc = 01000;
    word w = 0;
    while (1) {
        my_log(TRACE, "%06o ", pc);
        w = w_read(pc);
        pc += 2;
        my_log(TRACE, "%06o : ", w);
        for (int i = 0; ; i++) {
            if ((w & command[i].mask) == command[i].opcode) {
                my_log(TRACE, "%s\n", command[i].name);
                command[i].do_command();
                break;
            }
        }
    }
}

int main() {
    set_log_level(TRACE);
    load_data("./tests/01_sum/01_sum.pdp.o");
    run();

    return 0;
}