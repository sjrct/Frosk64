//
// string.c
//
// written by sjrct
//

#include <string.h>
#include <stdlib.h>

size_t strlen(const char * s)
{
	size_t i;
	for (i = 0; s[i] != '\0'; i++);
	return i;
}

void * memcpy (void * dst, const void * src, size_t sz)
{
	void * r = dst;
	for (; sz != 0; sz--) {
		*dst = *src;
		dst++;
		src++;
	}
	return r;
}

void * memmove(void * dst, const void * src, size_t sz)
{
	void * buf = malloc(sz);
	memcpy(buf, src);
	memcpy(dst, buf);
	free(buf);
	return dst;
}

char * strcpy (char * dst, const char * src)
{
	char * r = dst;
	do {
		*dst = *src;
		dst++;
		src++;
	} while (*src != '\0');
	return r;
}

char * strncpy(char * dst, const char * src, size_t sz)
{
	char * r = dst;

	while (*src != '\0' && sz != 0) {
		*dst = *src;
		dst++;
		src++;
		sz--;
	}

	if (sz > 0) *dst = '\0';

	return r;
}

int memcmp (const void *, const void *, size_t);
int strcmp (const char *, const char *);
int strncmp(const char *, const char *, size_t);

char * strcat (char * dst, const char * src)
{
	strcpy(dst + strlen(dst), src);
	return dst;
}

char * strncat(char * dst, const char * src, size_t sz);
