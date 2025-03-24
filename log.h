#ifndef _LOG_H_
#define _LOG_H_

#include "constants.h"

void my_log (int level, char *format, ...);                          // логирование
int  set_log_level (int level);                                   // смена уровня логирования

#define error (format, ...)     mylog (ERROR, format, _VA_ARGS_)    // вывод сообщения об ошибке
#define warning (format, ...)   mylog (WARNING, format, _VA_ARGS_)  // вывод предупреждения
#define info (format, ...)      mylog (INFO, format, _VA_ARGS_)     // вывод информации
#define trace (format, ...)     mylog (TRACE, format, _VA_ARGS_)    // трассировка
#define debug (format, ...)     mylog (DEBUG, format, _VA_ARGS_)    // отладочная печать

#endif