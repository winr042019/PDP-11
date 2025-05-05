#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "memory.h"
#include "log.h"
#include "command.h"

extern word reg[REGSIZE];

void run() {
    pc = 01000;
    b_write(OSTAT, 0xFF);
    Command cmd;
    while (1) {
        cmd = parse_cmd(read_cmd()); // читаем слово и разбираем команду
        cmd.do_command();
        my_log(TRACE, "\n");
        reg_dump();
    }
}

int main() {
    set_log_level(TRACE);
    load_data("./tests/08_hello/08_hello.pdp.o");
    run();

    return 0;
}