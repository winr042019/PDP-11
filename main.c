#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

typedef unsigned char byte;
typedef unsigned short int word;
typedef word address;

#define MEMSIZE (64*1024)

enum log_levels {
    ERROR    = 0,
    WARNING  = 1,
    INFO     = 2,
    TRACE    = 3,
    DEBUG    = 4
};

byte mem[MEMSIZE];

int log_level = ERROR;

void b_write (address adr, byte val);                             //  пишем значение (байт) val по адресу adr
byte b_read (address adr);                                        //  читаем байт по адресу adr и возвращаем его
void w_write (address adr, word val);                             //  пишем значение (слово) val по адресу adr
word w_read (address adr);                                        //  читаем слово по адресу adr и возвращаем его
void load_file(FILE * file);                                      //  считываем данные из файла
void load_data(const char * filename);                            //  считываем данные записываем их в массив mem
void mem_dump(address adr, int size);                             //  печатаем часть массива mem

void log (int level, char *format, ...);                          // логирование
int  set_log_level (int level);                                   // смена уровня логирования

#define error (format, ...)     log (ERROR, format, _VA_ARGS_)    // вывод сообщения об ошибке
#define warning (format, ...)   log (WARNING, format, _VA_ARGS_)  // вывод предупреждения
#define info (format, ...)      log (INFO, format, _VA_ARGS_)     // вывод информации
#define trace (format, ...)     log (TRACE, format, _VA_ARGS_)    // трассировка
#define debug (format, ...)     log (DEBUG, format, _VA_ARGS_)    // отладочная печать

void test_mem() {
    address a;
    byte b0, b1, bres;
    word w, wres;


    // пишем байт, читаем байт
    fprintf(stderr, "Пишем и читаем байт по четному адресу\n");
    a = 0;
    b0 = 0x12;
    b_write(a, b0);
    bres = b_read(a);
    // тут полезно написать отладочную печать a, b0, bres
    fprintf(stderr, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
    assert(b0 == bres);
    // аналогично стоит проверить чтение и запись по нечетному адресу


    // пишем слово, читаем слово
    fprintf(stderr, "Пишем и читаем слово\n");
    a = 2;        // другой адрес
    w = 0x3456;
    w_write(a, w);
    wres = w_read(a);
    // тут полезно написать отладочную печать a, w, wres
    fprintf(stderr, "a=%06o w=%04x wres=%04x\n", a, w, wres);
    assert(w == wres);


    // пишем 2 байта, читаем 1 слово
    fprintf(stderr, "Пишем 2 байта, читаем слово\n");
    a = 4;        // другой адрес
    w = 0xa1b2;
    // little-endian, младшие разряды по меньшему адресу
    b0 = 0xb2;
    b1 = 0xa1;
    b_write(a, b0);
    b_write(a+1, b1);
    wres = w_read(a);
    // тут полезно написать отладочную печать a, w, wres
    fprintf(stderr, "a=%06o w=%04x wres=%04x\n", a, w, wres);
    fprintf(stderr, "a=%06o b1=%02hhx b0=%02hhx wres=%04hx\n", a, b1, b0, wres);
    assert(w == wres);

   // пишем 1 слово, читаем 2 байта
    fprintf(stderr, "Пишем слово, читаем 2 байта \n");
    a = 6;        // другой адрес
    w = 0xa1b2;
    // little-endian, младшие разряды по меньшему адресу
    b0 = 0xb2;
    b1 = 0xa1;
    w_write(a, w);
    byte b0res = b_read(a);
    byte b1res = b_read(a+1);
    wres = w_read(a);
    // тут полезно написать отладочную печать a, w, wres
    fprintf(stderr, "a=%06o b1=%02hhx b0=%02hhx wres=%04x\n", a, b1, b0, w);
    assert(b0 == b0res);
    assert(b1 == b1res);
}

int main() {
    // test_mem();

    load_data("data.txt");

    mem_dump(0x40, 20);
    printf("\n");
    mem_dump(0x200, 0x26);

    return 0;
}

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

void log(int level, char * format, ...) {
    if (level > log_level)
        return;
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}

int  set_log_level (int level) {
    int old_log_level = log_level;
    log_level = level;
    return old_log_level;
}
