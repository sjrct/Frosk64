#include "inline_asm.h"
#include "kerndef.h"
#define PORT 0x3f8   /* COM1 */

int is_transmit_empty() {
	uchar a;
	INB(a, PORT + 5);
	return a & 0x20;
}
 
void write_serial(char a) {
	while (is_transmit_empty() == 0);
 
	OUTB(PORT,a);
}
