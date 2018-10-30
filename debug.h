
#ifndef DEBUG_R_
#define DEBUG_R_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>


#define err(msg, ...) fprintf(stderr, "[ERROR]" msg " in %s:%d:%s "  "\n",\
                __FILE__, __LINE__, __func__,##__VA_ARGS__)

#define info(msg, ...) fprintf(stderr, "[INFO] " msg "\n", ##__VA_ARGS__)

#define check(test, msg, ...) if(!(test)) {err(msg, ##__VA_ARGS__);}//goto error_handler;}

#define debug(msg, ...) fprintf(stderr, "[DEBUG] " msg "\n", ##__VA_ARGS__)


#endif 