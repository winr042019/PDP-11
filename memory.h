#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdio.h>

#include "constants.h"

extern word reg[REGSIZE];
#define pc reg[7]

void b_write (address adr, byte val);                       //  пишем значение (байт) val по адресу adr
byte b_read (address adr);                                  //  читаем байт по адресу adr и возвращаем его
void w_write (address adr, word val);                       //  пишем значение (слово) val по адресу adr
word w_read (address adr);                                  //  читаем слово по адресу adr и возвращаем его
void load_file(FILE * file);                                //  считываем данные из файла
void load_data(const char * filename);                      //  считываем данные записываем их в массив mem
void mem_dump(address adr, int size);                       //  печатаем часть массива mem

#endif