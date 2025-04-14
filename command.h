#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "constants.h"

extern Command command[];

Argument get_arg(word w);
word read_cmd();
Command parse_cmd(word cmd);
void do_add();
void do_clr();
void do_halt();
void do_mov();
void do_sob();
void do_unknown();

#endif