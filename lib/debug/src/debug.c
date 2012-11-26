#include <frosk.h>

void debug_string(const char * s) {
	for(; *s != '\0'; s++) {
		write_serial(*s);
	}
}

void debug_line(const char * s) {
	debug_string(s);
	write_serial('\n');
}

char *itoa(int value, char *s, int radix)
{
    const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    unsigned long ulvalue = value;
    char *p = s, *q = s;
    char temp;
    if (radix == 10 && value < 0) {
        *p++ = '-';
        q = p;
        ulvalue = -value;
    }
    do {
        *p++ = digits[ulvalue % radix];
        ulvalue /= radix;
    } while (ulvalue > 0);
    *p-- = '\0';
    while (q < p) {
        temp = *q;
        *q++ = *p;
        *p-- = temp;
    }
    return s;
}

void debug_number(int num) {
	char * s = "________________________________";
	s = itoa(num, s, 10);
	debug_line(s);
}

void debug_hex(int num) {
	char * s = "________________________________";
	s = itoa(num, s, 16);
	debug_string(s);
}

