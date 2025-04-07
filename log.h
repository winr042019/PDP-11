#ifndef _LOG_H_
#define _LOG_H_

#include "constants.h"

void my_log (int level, char *format, ...);                             // логирование
int  set_log_level (int level);                                         // смена уровня логирования
void reg_dump();

#define error(format, ...)       my_log(ERROR, format, __VA_ARGS__)     // вывод сообщения об ошибке
#define warning(format, ...)     my_log(WARNING, format, __VA_ARGS__)   // вывод предупреждения
#define info(format, ...)        my_log(INFO, format, __VA_ARGS__)      // вывод информации
#define trace(format, ...)       my_log(TRACE, format, __VA_ARGS__)     // трассировка
#define debug(format, ...)       my_log(DEBUG, format, __VA_ARGS__)     // отладочная печать

#endif