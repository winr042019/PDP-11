#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

typedef unsigned char byte;
typedef unsigned short int word;
typedef word address;

typedef struct {
    word mask;
    word opcode;
    char * name;
    void (*do_command)(void);
} Command;

#define REGSIZE 8
#define MEMSIZE (64*1024)

enum log_levels {
    ERROR    = 0,
    WARNING  = 1,
    INFO     = 2,
    TRACE    = 3,
    DEBUG    = 4
};

#endif