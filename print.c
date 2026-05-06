#include "sciTinyTimber.h"
#include <stdarg.h>
#include <stdio.h>

#define BUFFER_SIZE ((unsigned int)1024)

extern Serial sci0;

void print(char *format, ...) {
  char buffer[BUFFER_SIZE];
  va_list argp;
  va_start(argp, format);
  vsnprintf(buffer, BUFFER_SIZE, format, argp);
  va_end(argp);
  SCI_WRITE(&sci0, buffer);
}
