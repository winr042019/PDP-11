#include <stdlib.h>

#include "command.h"
#include "constants.h"
#include "log.h"
#include "memory.h"

Argument ss, dd;

extern word reg[REGSIZE];

Command command[] = {
    {0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
    {0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},
    {0000000, 0000000, "unknown", do_unknown, NO_PARAMS} // LAST
};

Argument get_arg(word w) {
    Argument arg;
    int r = w & 7;
    int m = (w >> 3) & 7;
    switch (m) {
    case 0:
        arg.addr = r;
        arg.value = reg[r];
        my_log(TRACE, "R%d ", r);
        break;
    case 1:
        arg.addr = reg[r];
        arg.value = w_read(arg.addr);
        my_log(TRACE, "(R%d) ", r);
        break;
    case 2:
        arg.addr = reg[r];
        arg.value = w_read(arg.addr);
        reg[r] += 2;
        if (r == 7)
            my_log(TRACE, "#%o ", arg.value);
        else
            my_log(TRACE, "(R%d)+ ", r);
        break;
    default:
        my_log(ERROR, "Not implemented\n");
        exit(1);
    }
    return arg;
}

word read_cmd() {
    word w = 0;
    my_log(TRACE, "\n%06o ", pc);
    w = w_read(pc);
    pc += 2;
    my_log(TRACE, "%06o : ", w);
    return w;
}

Command parse_cmd(word cmd) {
    for (int i = 0; ; i++) {
        if ((cmd & command[i].mask) == command[i].opcode) {
            my_log(TRACE, "%s ", command[i].name);
            if (command[i].params & HAS_SS) {
                ss = get_arg(cmd >> 6);
            } if (command[i].params & HAS_DD) {
                dd = get_arg(cmd);
            }
            return command[i];
        }
    }
}

void do_halt() {
    reg_dump();
    my_log(TRACE, "END\n");
    exit(0);
}

void do_add() {
    if (dd.addr < 8)
        reg[dd.addr] = dd.value + ss.value;
    else
        w_write(dd.addr, dd.value + ss.value);
}

void do_mov() {
    if (dd.addr < 8)
        reg[dd.addr] = ss.value;
    else
        w_write(dd.addr, ss.value);
}

void do_unknown() {
    my_log(TRACE, "Not implemented\n");
}