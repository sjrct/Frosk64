//
// error.c
//
// written by sjrct
//

#include <stdio.h>
#include <stdarg.h>
#include "error.h"

void repor_err(err_st * es, const char * fmt,...)
{
	va_list vl;
	
	va_start(vl, fmt);
	
	if (es->fn == NULL) fprintf(es->f, "Error: ");
	else fprintf(es->f, "Error: %s: line %d: ", es->fn, es->ln);

	vfprintf(es->f, fmt, vl);
	
	va_end(vl);
}

void repor_warn(err_st * es, const char * fmt,...)
{
	va_list vl;
	
	va_start(vl, fmt);
	
	if (es->fn == NULL) fprintf(es->f, "Error: ");
	else fprintf(es->f, "Error: %s: line %d: ", es->fn, es->ln);

	vfprintf(es->f, fmt, vl);
	
	va_end(vl);
}
