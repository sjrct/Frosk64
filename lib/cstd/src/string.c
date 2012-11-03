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

void * memcpy (void * vdst, const void * vsrc, size_t sz)
{
	void * r = vdst;
	char * dst = vdst;
	const char * src = vsrc;
	
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
	memcpy(buf, src, sz);
	memcpy(dst, buf, sz);
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

int memcmp(const void * s1, const void * s2, size_t sz)
{
	size_t i;
	const unsigned char * u1 = s1;
	const unsigned char * u2 = s2;

	for (i = 0; i < sz; i++) {
		if (u1[i] != u2[i]) {
			return u1[i] > u2[i] ? 1 : -1;
		}
	}
	return 0;
}

int strcmp(const char * s1, const char * s2)
{
	size_t i;
	for (i = 0; s1[i] != '\0'; i++) {
		if (s1[i] != s2[i]) {
			return s1[i] > s2[i] ? 1 : -1;
		}
	}
	return 0;	
}

int strncmp(const char * s1, const char * s2, size_t sz)
{
	size_t i;
	for (i = 0; i < sz && s1[i] != '\0'; i++) {
		if (s1[i] != s2[i]) {
			return s1[i] > s2[i] ? 1 : -1;
		}
	}
	return 0;	
}


char * strcat (char * dst, const char * src)
{
	strcpy(dst + strlen(dst), src);
	return dst;
}

char * strncat(char * dst, const char * src, size_t sz)
{
	strncpy(dst + strlen(dst), src, sz);
	return dst;
}
