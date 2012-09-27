//
// kstrlib.c
//
// written by sjrct
//

#include "kstrlib.h"

int strlen(const char * s)
{
	int i = 0;
	while (s[i] != 0) i++;
	return i;
}

int memcmp(const char * s1, const char * s2, uint sz)
{
	uint i;
	for (i = 0; i < sz; i++) {
		if (s1[i] != s2[i]) return i + 1;
	}
	return 0;
}

int strncmp(const char * s1, const char * s2, uint sz)
{
	uint i;
	for (i = 0; s1[i] != '\0' && i < sz; i++) {
		if (s1[i] != s2[i]) return i + 1;
	}
	return 0;
}

