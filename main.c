#include <stdio.h>
#include <assert.h>

#include "constants.h"
#include "memory.h"
#include "log.h"

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
    set_log_level(INFO);

    int x, y;
    scanf("%d%d", &x, &y);

    my_log(INFO, "%d + %d = %d\n", x, y, x + y - 1);
    my_log(ERROR, "Oops, %d+%d=%d, not %d\n", x, y, x + y, x + y - 1);
    my_log(TRACE, "Эту надпись не должны видеть\n");

    int old_log_level = set_log_level(TRACE);

    my_log(TRACE, "Visible text\n");
    my_log(DEBUG, "Debug info\n");

    set_log_level(old_log_level);

    my_log(INFO, "The end!\n");
    my_log(TRACE, "No code after return\n");

    return 0;
}