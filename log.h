#ifndef _LOG_H_
#define _LOG_H_

#include "constants.h"

void my_log (int level, char *format, ...);                            // логирование
int  set_log_level (int level);                                        // смена уровня логирования

#define error   (format, ...)     my_log (ERROR, format, _VA_ARGS_)    // вывод сообщения об ошибке
#define warning (format, ...)     my_log (WARNING, format, _VA_ARGS_)  // вывод предупреждения
#define info    (format, ...)     my_log (INFO, format, _VA_ARGS_)     // вывод информации
#define trace   (format, ...)     my_log (TRACE, format, _VA_ARGS_)    // трассировка
#define debug   (format, ...)     my_log (DEBUG, format, _VA_ARGS_)    // отладочная печать

#endif