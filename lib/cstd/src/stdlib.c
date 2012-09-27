//
// stdlib.c
//
// written by sjrct
//

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

long atol(const char * s)
{
	bool isneg = false;
	long t = 0;

	while (isspace(*s)) s++;
	if (*s == '-') {
		isneg = true;
		s++;
	}
	
	while (isdigit(*s)) {
		t *= 10;
		t += (long)(*s - '0');
		s++;
	}
	
	if (isneg) t = -t;
	return t;
}

int atoi(const char * s)
{
	return (int)atol(s);
}

int abs(int x)
{
	if (x < 0) return -x;
	return x;
}

long labs(long x)
{
	if (x < 0) return -x;
	return x;
}

div_t div(int n, int d)
{
	div_t r;
	r.quot = n / d;
	r.rem = n / d;
	return r;
}

ldiv_t ldiv(long n, long d)
{
	div_t r;
	r.quot = n / d;
	r.rem = n / d;
	return r;
}
