#include <stdlib.h>

#include "command.h"
#include "constants.h"
#include "log.h"
#include "memory.h"

Argument ss, dd;
int r, nn;
char xx;
char is_byte;
char flags;         // NZVC

#define Npos    (1<<3)
#define Zpos    (1<<2)
#define Vpos    (1<<1)
#define Cpos    (1)
#define SETN(x) ((x) | Npos)
#define CLRN(x) ((x) & ~Npos)
#define WSIGN   (1 << 15)
#define BSIGN   (1 << 7)
#define SETZ(x) ((x) | Zpos)
#define CLRZ(x) ((x) & ~Zpos)
#define WCARRY  (1 << 16)
#define BCARRY  (1 << 8)
#define SETC(x) ((x) | Cpos)
#define CLRC(x) ((x) & ~Cpos)

extern word reg[REGSIZE];

Command command[] = {
    {0170000, 0060000, "add",     do_add,     HAS_DD | HAS_SS        },
    {0177700, 0103000, "bcc",     do_bcc,     HAS_XX                 },
    {0177700, 0103400, "bcs",     do_bcs,     HAS_XX                 },
    {0177700, 0001400, "beq",     do_beq,     HAS_XX                 },
    {0177700, 0002000, "bge",     do_bge,     HAS_XX                 },
    {0177700, 0003000, "bgt",     do_bgt,     HAS_XX                 },
    {0177700, 0101000, "bhi",     do_bhi,     HAS_XX                 },
    {0177700, 0103000, "bhis",    do_bhis,    HAS_XX                 },
    {0177700, 0003400, "ble",     do_ble,     HAS_XX                 },
    {0177700, 0002400, "blt",     do_blt,     HAS_XX                 },
    {0177700, 0103400, "blo",     do_blo,     HAS_XX                 },
    {0177700, 0101400, "blos",    do_blos,    HAS_XX                 },
    {0177700, 0100400, "bmi",     do_bmi,     HAS_XX                 },
    {0177700, 0001000, "bne",     do_bne,     HAS_XX                 },
    {0177700, 0100000, "bpl",     do_bpl,     HAS_XX                 },
    {0177700, 0000400, "br",      do_br,      HAS_XX                 },
    {0177700, 0102000, "bvc",     do_bvc,     HAS_XX                 },
    {0177700, 0102400, "bvs",     do_bvs,     HAS_XX                 },
    {0077700, 0005000, "clr",     do_clr,     HAS_DD                 },
    {0170000, 0010000, "mov",     do_mov,     HAS_DD | HAS_SS        },
    {0170000, 0110000, "movb",    do_mov,     BYTE | HAS_DD | HAS_SS },
    {0177777, 0000000, "halt",    do_halt,    NO_PARAMS              },
    {0177000, 0077000, "sob",     do_sob,     HAS_R | HAS_NN         },
    {0000000, 0000000, "unknown", do_unknown, NO_PARAMS              } // LAST
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
        arg.value = is_byte ? b_read(arg.addr) : w_read(arg.addr);
        my_log(TRACE, "(R%d) ", rg);
        break;
    case 2:
        arg.addr = reg[rg];
        arg.value = is_byte ? b_read(arg.addr) : w_read(arg.addr);
        if (is_byte && rg < 6)
            reg[rg] += 1;
        else
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
            is_byte = command[i].params & BYTE;
            if (is_byte)
                my_log(TRACE, "byte ");
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
            if (command[i].params & HAS_XX) {
                xx = cmd & 077;
            }
            return command[i];
        }
    }
}

void do_add() {
    dword res = dd.value + ss.value;
    if (dd.addr < 8)
        reg[dd.addr] = res;
    else
        w_write(dd.addr, res);
    set_nz(res);
    set_c(res);
}

void do_bcc() {
    if (!(flags & Cpos))
        do_br();
}

void do_bcs() {
    if (flags & Cpos)
        do_br();
}

void do_beq() {
    if (flags & Zpos)
        do_br();
}

void do_bge() {
    if (!((flags & Npos) & (flags & Vpos)))
        do_br();
}

void do_bgt() {
    if (!((flags & Zpos) | ((flags & Npos) & (flags & Vpos))))
        do_br();
}

void do_bhi() {
    if (!((flags & Cpos) | (flags & Zpos)))
        do_br();
}

void do_bhis() {
    if (!(flags & Cpos))
        do_br();
}

void do_ble() {
    if ((flags & Zpos) | ((flags & Npos) & (flags & Vpos)))
        do_br();
}

void do_blt() {
    if ((flags & Npos) & (flags & Vpos))
        do_br();
}

void do_blo() {
    if (flags & Cpos)
        do_br();
}

void do_blos() {
    if ((flags & Cpos) | (flags & Zpos))
        do_br();
}

void do_bmi() {
    if (flags & Npos)
        do_br();
}

void do_bne() {
    if (!(flags & Zpos))
        do_br();
}

void do_bpl() {
    if (!(flags & Npos))
        do_br();
}

void do_br() {
    pc = pc + xx * 2;
}

void do_bvc() {
    if (!(flags & Vpos))
        do_br();
}

void do_bvs() {
    if (flags & Vpos)
        do_br();
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
    if (is_byte)
        b_write(dd.addr, ss.value);
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

void set_nz(dword res) {
    if (is_byte)
        flags = res & BSIGN ? SETN(flags) : CLRN(flags);
    else
        flags = res & WSIGN ? SETN(flags) : CLRN(flags);
    flags = res ? SETZ(flags) : CLRZ(flags);
}

void set_c(dword res) {
    if (is_byte)
        flags = res & BCARRY ? SETC(flags) : CLRC(flags);
    else
        flags = res & WCARRY ? SETC(flags) : CLRC(flags);
}