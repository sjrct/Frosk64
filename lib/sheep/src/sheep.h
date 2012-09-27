//
// sheep.h
//
// written by sjrct
//

#ifndef _SHEEP_H_
#define _SHEEP_H_

#include <stdio.h>

struct sheep_sck;

typedef struct sheep_var {
	char * name;
	
	enum {
		SHEEP_STR,
		SHEEP_SCK,
	} type;
	
	union {
		char * str;
		struct sheep_sck * sck;
	} u;
} sheep_var;

typedef struct sheep_sck {
	size_t size;
	sheep_var ** vars;
} sheep_sck;

#define SHEEP_IS_STR(X) (X->type == SHEEP_STR)
#define SHEEP_IS_SCK(X) (X->type == SHEEP_SCK)

//
// sheep_exec_file: Evaluates and parses a file
//
// Parameters:
//   const char * filename
//   FILE * error_file
//
// Returns 0 on error, 1 on success
//
int sheep_exec_file(const char *, FILE *);

//
// sheep_exec_file: Evaluates and parses a file
//
// Parameters:
//   const char * line_buffer
//   FILE * error_file
//
// Returns 0 on error, 1 on success
//
int sheep_exec_line(const char *, FILE *);

//
// sheep_new_str: Allocates a new string variable
//
// Parameters:
//   const char * name
//   const char * value
//
// Returns: New variable
//
sheep_var * sheep_new_str(const char *, const char *);

//
// sheep_new_sck: Allocates a new empty sack variable
//
// Parameters:
//   const char * name
//
// Returns: New variable
//
sheep_var * sheep_new_sck(const char *);

//
// sheep_new_arr: Allocates a new sack variable with all element names being
//                sequential numbers starting from 0.
//
// Parameters:
//   const char * name
//   int element_count;
//   const char ** strings
//
// Returns: New variable
//
sheep_var * sheep_new_arr(const char *, int, const char **);

sheep_var * sheep_copy_var(const sheep_var *);
void sheep_free_var(sheep_var *);

void sheep_set_var(const sheep_var *);
const sheep_var * sheep_get_var(const char *);
void sheep_set_elm(sheep_var *, const sheep_var *);
const sheep_var * sheep_get_elm(sheep_var *, const char *);

#endif
