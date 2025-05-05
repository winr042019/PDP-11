#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "constants.h"

extern Command command[];

Argument get_arg(word w);
word read_cmd();
Command parse_cmd(word cmd);
void do_add();
void do_bcc();
void do_bcs();
void do_beq();
void do_bge();
void do_bgt();
void do_bhi();
void do_bhis();
void do_ble();
void do_blt();
void do_blo();
void do_blos();
void do_bmi();
void do_bne();
void do_bpl();
void do_br();
void do_bvc();
void do_bvs();
void do_clr();
void do_halt();
void do_jmp();
void do_mov();
void do_sob();
void do_unknown();
void set_nz(dword res);
void set_c(dword res);

#endif