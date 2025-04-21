#include <stdio.h>

#include "constants.h"
#include "memory.h"
#include "log.h"


static byte mem[MEMSIZE];

word reg[REGSIZE];

void b_write (address adr, byte val) {
    if (adr < 8) {
        reg[adr] = (val & 0x100) == 0x100 ? val | 0xFF : val & 0xF;
    }
    else {
        mem[adr] = val;
    }
}

byte b_read (address adr) {
    return adr < 8 ? reg[adr] & 0xFF : mem[adr];
}

void w_write (address adr, word val) {
    if (adr < 8) {
        reg[adr] = val;
    } else {
        mem[adr] = val & 255;
        mem[adr + 1] = val >> 8;
    }
}

word w_read (address adr) {
    if (adr < 8) {
        return reg[adr];
    } else {
        word w = mem[adr + 1] << 8;
        w |= mem[adr] & 255;
        return w & 0xFFFF;
    }
}

void load_file(FILE * file) {
    address adr = 0;
    int N = 0;
    byte b = 0;
    while (fscanf(file, "%hx", &adr) != EOF) {
        fscanf(file, "%x", &N);
        for (int i = 0; i < N; i++) {
            fscanf(file, "%02hhx", &b);
            b_write(adr + i, b);
        }
    }
}

void load_data(const char * filename) {
    FILE * file  = fopen(filename, "r");
    if (file == NULL)
        perror(filename);
    load_file(file);
    fclose(file);
}

void mem_dump(address adr, int size) {
    for (int i = 0; i < size; i += 2) {
        trace("%06o: %06o %04x\n", adr + i, w_read(adr + i), w_read(adr + i)); 
    }
}