#ifndef _LOG_H_
#define _LOG_H_

#include "constants.h"

void log (int level, char *format, ...);                          // логирование
int  set_log_level (int level);                                   // смена уровня логирования

#define error (format, ...)     log (ERROR, format, _VA_ARGS_)    // вывод сообщения об ошибке
#define warning (format, ...)   log (WARNING, format, _VA_ARGS_)  // вывод предупреждения
#define info (format, ...)      log (INFO, format, _VA_ARGS_)     // вывод информации
#define trace (format, ...)     log (TRACE, format, _VA_ARGS_)    // трассировка
#define debug (format, ...)     log (DEBUG, format, _VA_ARGS_)    // отладочная печать

#endif