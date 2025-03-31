#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "constants.h"

extern Command command[];

void do_halt();
void do_add();
void do_mov();
void do_unknown();

#endif