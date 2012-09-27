//
// stdlib.h
//
// written by sjrct
//

#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stddef.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE (-1)

#define RAND_MAX 0xfFFFfFFF

typedef struct div_t {
	int quot, rem;
} div_t;

typedef struct ldiv_t {
	long quot, rem;
} ldiv_t;

void * malloc(size_t);
void * realloc(void *, size_t);
void * calloc(size_t, size_t);
void free(void *);

void srand(unsigned);
unsigned rand(void);

double atof(const char *);
int atoi(const char *);
long atol(const char *);

int abs(int);
long labs(long);

div_t div(int, int);
ldiv_t ldiv(long, long);

#endif
