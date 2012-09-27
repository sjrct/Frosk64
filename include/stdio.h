//
// stdio.h
//
// written by sjrct
//

#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>
#include <streams.h>

int fprintf(FILE *, const char *,...);
int fscanf(FILE *, const char *,...);
int printf(const char *,...);
int scanf(const char *,...);
int vfprintf(FILE *, const char *, va_list);
int vprintf(const char *, va_list);

#endif
