#include <stdlib.h>

#include "command.h"
#include "constants.h"
#include "log.h"

Command command[] = {
    {0170000, 0060000, "add", do_add},
    {0170000, 0010000, "mov", do_mov},
    {0177777, 0000000, "halt", do_halt},
    {0000000, 0000000, "unknown", do_unknown} // LAST
};

void do_halt() {
    my_log(TRACE, "END\n");
    exit(0);
}

void do_add() {

}

void do_mov() {
    
}

void do_unknown() {

}