//
// kstrlib.c
//
// written by sjrct
//

#include "kstrlib.h"

int memcmp(const char * s1, const char * s2, uint sz)
{
	uint i;
	for (i = 0; i < sz; i++) {
		if (s1[i] != s2[i]) return i + 1;
	}
	return 0;
}
