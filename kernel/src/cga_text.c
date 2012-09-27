//
// cga_text.c
//
// written by sjrct
//

#include "kerndef.h"
#include "cga_text.h"

static int init(void);
static int cls(long);
static int putc(long);
//static 
int puts(const char *);
//static 
int putu(unumber *);
static int putl(snumber *);
//static 
int putnl(void);

int (*cga_text_functions[CGA_TEXT_FUNC_COUNT])() = {
	init,
	cls,
	putc,
	puts,
	putu,
	putl,
	putnl,
};

#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25
#define SCREEN_BUFFER (KSPACE_LOC + 0xB8000)
#define SCREEN_BUFFER_TOP (SCREEN_BUFFER + SCREEN_WIDTH * SCREEN_HEIGHT * 2)

#define DEFAULT_CLS 0x0F

static char * cursor;
static int cleared;

static int init(void)
{
	cls(DEFAULT_CLS);
	return 0;
}

static int cls(long col)
{
	char * b;

	for (b = (char*)SCREEN_BUFFER; b != (char*)SCREEN_BUFFER_TOP; b+=2) {
		b[0] = 0;
		b[1] = col;
	}

	cursor = (char*)SCREEN_BUFFER;
	cleared = 1;

	return 0;
}

static int putc(long c)
{
	if (!cleared) cls(DEFAULT_CLS);

	if (c == '\n') {
		putnl();
	} else {
		*cursor = c;
		cursor += 2;
	}

	return 0;
}

//static 
int puts(const char * str)
{
	if (!cleared) cls(DEFAULT_CLS);
	
	while (*str != '\0') {
		if (*str == '\n') {
			putnl();
		} else {
			*cursor = *str;
			cursor += 2;
		}
		str++;
	}

	return 0;
}

//static 
int putu(unumber * u)
{
	int l, i;
	char c;
	ulong v2;

	if (!cleared) cls(DEFAULT_CLS);

	if (u->n == 0) {
		*cursor = '0';
		cursor += 2;
		return 0;
	}

	i = l = 0;
	v2 = u->n;

	while (v2 != 0) {
		v2 /= u->b;
		l++;
	}

	while (u->n != 0) {
		c = u->n % u->b;
		u->n /= u->b;
		if (c > 9) c = 'A' - 10 + c;
		else c = '0' + c;
		i++;
		cursor[(l - i) * 2] = c;
	}

	cursor += l * 2;

	return 0;
}

static int putl(snumber * s)
{
	snumber s2 = *s;

	if (!cleared) cls(DEFAULT_CLS);
	if (s2.n < 0) {
		*cursor = '-';
		cursor += 2;
		s2.n = -s2.n;
	}

	putu((unumber*)(&s2));

	return 0;
}

//static 
int putnl(void)
{
	if (!cleared) cls(DEFAULT_CLS);

	while (((ulong)cursor - SCREEN_BUFFER) % (SCREEN_WIDTH * 2) != 0) {
		cursor += 2;
	}

	return 0;
}
