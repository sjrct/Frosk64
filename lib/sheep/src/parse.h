//
// parse.h
//
// written by sjrct
//

#ifndef PARSE_H
#define PARSE_H

#include "error.h"

typedef struct {
	int argc;
	char ** argv;
} args;

args next_statement(const char **, err_st *);
void remove_comments(char * buf);
void free_args(args);

#endif
