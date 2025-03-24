#include <stdio.h>

#include "constants.h"
#include "memory.h"

static byte mem[MEMSIZE];

void b_write (address adr, byte val) {
    mem[adr] = val;
}

byte b_read (address adr) {
    return mem[adr];
}

void w_write (address adr, word val) {
    mem[adr] = val & 255;
    mem[adr + 1] = val >> 8;
}

word w_read (address adr) {
    word w = mem[adr + 1] << 8;
    w |= mem[adr] & 255;
    return w & 0xFFFF;
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
        printf("%06o: %06o %04x\n", adr + i, w_read(adr + i), w_read(adr + i)); 
    }
}