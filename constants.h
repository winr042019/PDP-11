#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;
typedef word address;

typedef struct {
    word mask;
    word opcode;
    char * name;
    void (*do_command)(void);
    char params;
} Command;

typedef struct {
    word value;
    address addr;
} Argument;

#define REGSIZE 8
#define MEMSIZE (64*1024)

#define OSTAT 0177564
#define ODATA 0177566

#define Npos    (1<<3)
#define Zpos    (1<<2)
#define Vpos    (1<<1)
#define Cpos    (1)

enum log_levels {
    ERROR    = 0,
    WARNING  = 1,
    INFO     = 2,
    TRACE    = 3,
    DEBUG    = 4
};

enum command_params {
    NO_PARAMS = 0,
    BYTE      = 1,
    HAS_DD    = 2,
    HAS_SS    = 4,
    HAS_R     = 8,
    HAS_NN    = 16,
    HAS_XX    = 32
};

enum flag_values {
    N = 0,
    Z = 1, 
    C = 2,
    V = 4
};

#endif