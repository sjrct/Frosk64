//
// error.h
//
// written by sjrct
//

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

typedef struct {
	const char * fn;
	unsigned ln;
	FILE * f;
} err_st;

void repor_err (err_st *, const char *,...);
void repor_warn(err_st *, const char *,...);

#endif
