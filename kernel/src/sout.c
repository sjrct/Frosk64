//
// sout.c
//
// written by sjrct
//

#include "kerndef.h"
#include "sout.h"

#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25
#define SCREEN_BUFFER (KSPACE_LOC + 0xB8000)
#define SCREEN_BUFFER_TOP (SCREEN_BUFFER + SCREEN_WIDTH * SCREEN_HEIGHT * 2)

#define DEFAULT_CLS 0x0F

static char * cursor;
static int cleared;

void cls(char col)
{
	char * b;
	for (b = (char*)SCREEN_BUFFER; b != (char*)SCREEN_BUFFER_TOP; b+=2) {
		b[0] = 0;
		b[1] = col;
	}
	cursor = (char*)SCREEN_BUFFER;
	cleared = 1;
}

void outnl()
{
	if (!cleared) cls(DEFAULT_CLS);

	while (((ulong)cursor - SCREEN_BUFFER) % (SCREEN_WIDTH * 2) != 0) {
		cursor += 2;
	}
}

void outc(char c)
{
	if (!cleared) cls(DEFAULT_CLS);

	if (c == '\n') {
		outnl();
	} else {
		*cursor = c;
		cursor += 2;
	}
}

void outs(const char * str)
{
	if (!cleared) cls(DEFAULT_CLS);

	while (*str != '\0') {
		if (*str == '\n') {
			outnl();
		} else {
			*cursor = *str;
			cursor += 2;
		}
		str++;
	}
}

void outn(ulong v, int b)
{
	int l, i;
	char c;
	ulong v2;

	if (!cleared) cls(DEFAULT_CLS);

	if (v == 0) {
		*cursor = '0';
		cursor += 2;
		return;
	}

	i = l = 0;
	v2 = v;

	while (v2 != 0) {
		v2 /= b;
		l++;
	}

	while (v != 0) {
		c = v % b;
		v /= b;
		if (c > 9) c = 'A' - 10 + c;
		else c = '0' + c;
		i++;
		cursor[(l - i) * 2] = c;
	}

	cursor += l * 2;
}

void outl(long v, int b)
{
	if (!cleared) cls(DEFAULT_CLS);
	if (v < 0) {
		*cursor = '-';
		cursor += 2;
		v = -v;
	}
	outn((ulong)v, b);
}
