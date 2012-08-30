//
// error.c
//
// written by sjrct
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

int current_line = -1;

void error(const char * errmsg,...)
{
	va_list argls;
	
	va_start(argls, errmsg);
	fprintf(stderr, "Error (line %d): ", current_line);
	vfprintf(stderr, errmsg, argls);
	fputc('\n', stderr);
	va_end(argls);

	exit(-1);
}
