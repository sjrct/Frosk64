//
// stdlib.h
//
// written by sjrct
//

#ifndef _STDLIB_H_
#define _STDLIB_H_

void * malloc(size_t);
void * realloc(void *, size_t);
void * calloc(int, size_t);
void free(void *);

void srand(unsigned);
unsigned rand();

#endif
