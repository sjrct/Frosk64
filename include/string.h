//
// string.h
//
// written by sjrct
//

#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

size_t strlen(const char *);

void * memcpy (void *, const void *, size_t);
void * memmove(void *, const void *, size_t);
char * strcpy (char *, const char *);
char * strncpy(char *, const char *, size_t);

int memcmp (const void *, const void *, size_t);
int strcmp (const char *, const char *);
int strncmp(const char *, const char *, size_t);

char * strcat (char *, const char *);
char * strncat(char *, const char *, size_t);

#endif
