//
// parse.h
//
// written by sjrct
//

#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>

typedef enum {
	PST_FILE,
	PST_DIR,
	PST_INDIRECT
} ps_type;

typedef enum {
	PERM_NONE,
	PERM_OWNER,
	PERM_ALL
} perm;

typedef struct parse_struct {
	ps_type type;
	char * name;
	perm read_perm;
	perm write_perm;
	perm exec_perm;
	int owner;

	union {
		char * actual;
		char * dest;
		struct parse_struct * contents;
	} u;

	struct parse_struct * next;
} parse_struct;

parse_struct * parse_next(FILE *, perm, perm, perm);
void free_parse_struct(parse_struct *);

#endif
