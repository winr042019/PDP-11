#include <stdlib.h>

#include "command.h"
#include "constants.h"
#include "log.h"
#include "memory.h"

Argument ss, dd;
int r, nn;

extern word reg[REGSIZE];

Command command[] = {
    {0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
    {0077700, 0005000, "clr", do_clr, HAS_DD},
    {0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
    {0177777, 0000000, "halt", do_halt, NO_PARAMS},
    {0177000, 0077000, "sob", do_sob, HAS_R | HAS_NN},
    {0000000, 0000000, "unknown", do_unknown, NO_PARAMS} // LAST
};

Argument get_arg(word w) {
    Argument arg;
    int rg = w & 7;
    int m = (w >> 3) & 7;
    switch (m) {
    case 0:
        arg.addr = rg;
        arg.value = reg[rg];
        my_log(TRACE, "R%d ", rg);
        break;
    case 1:
        arg.addr = reg[rg];
        arg.value = w_read(arg.addr);
        my_log(TRACE, "(R%d) ", rg);
        break;
    case 2:
        arg.addr = reg[rg];
        arg.value = w_read(arg.addr);
        reg[rg] += 2;
        if (rg == 7)
            my_log(TRACE, "#%o ", arg.value);
        else
            my_log(TRACE, "(R%d)+ ", rg);
        break;
    case 3:
        arg.addr = w_read(reg[rg]);
        arg.value = w_read(arg.addr);
        reg[rg] += 2;
        if (rg == 7)
            my_log(TRACE, "@#%o ", arg.value);
        else
            my_log(TRACE, "@(R%d)+ ", rg);
        break;
    default:
        my_log(TRACE, "%d not implemented\n", m);
        exit(1);
    }
    return arg;
}

word read_cmd() {
    word w = 0;
    my_log(TRACE, "%06o ", pc);
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
            } 
            if (command[i].params & HAS_DD) {
                dd = get_arg(cmd);
            }
            if (command[i].params & HAS_R) {
                r = (cmd >> 6) & 7;
                my_log(TRACE, "R%d ", r);
            } 
            if (command[i].params & HAS_NN) {
                nn = cmd & 077;
                my_log(TRACE, "#%o ", pc - (nn << 1));
            }
            return command[i];
        }
    }
}

void do_add() {
    if (dd.addr < 8)
        reg[dd.addr] = dd.value + ss.value;
    else
        w_write(dd.addr, dd.value + ss.value);
}

void do_clr() {
    if (dd.addr < 8)
        reg[dd.addr] = 0;
    else
        w_write(dd.addr, 0);
}

void do_halt() {
    reg_dump();
    my_log(TRACE, "END\n");
    exit(0);
}

void do_mov() {
    if (dd.addr < 8)
        reg[dd.addr] = ss.value;
    else
        w_write(dd.addr, ss.value);
}

void do_sob() {
    if (--reg[r])
        pc = pc - (nn << 1);
}

void do_unknown() {
    my_log(TRACE, "Not implemented");
}