#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "memory.h"
#include "log.h"
#include "command.h"

extern word reg[REGSIZE];

void run() {
    pc = 01000;
    Command cmd;
    while (1) {
        cmd = parse_cmd(read_cmd()); // читаем слово и разбираем команду
        cmd.do_command(); 
    }
}

int main() {
    set_log_level(TRACE);
    load_data("./tests/01_sum/01_sum.pdp.o");
    run();

    return 0;
}