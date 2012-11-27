//
// keyboard.c
//
// written by sjrct
//

#include <cmn/io.h>
#include "keyboard.h"
#include "kerndef.h"
#include "interrupt.h"
#include "irq.h"
#include "inline_asm.h"

char key_buf[KEY_BUF_MAX_SIZE];
uint key_buf_top = 0;
uint key_buf_bot = 0;

void kb_init(void)
{
	void key_int();
	char a;
	
	register_int(IRQ0 + IRQ_KEYBOARD, (ulong)key_int, KERNEL_CS, 0x8E);
	enable_irq(IRQ_KEYBOARD);

	INB(a, 0x64);
	while (a & 1) {
		INB(a, 0x60);
		INB(a, 0x64);
	}
}

int kb_read(char * buf, uint size)
{
	uint i;

	for (i = 0; i < size; i++) {
		if (key_buf_top != key_buf_bot) {
			buf[i] = key_buf[key_buf_bot];
			key_buf_bot++;
			if (key_buf_bot == KEY_BUF_MAX_SIZE) key_buf_bot = 0;
		} else {
			break;
		}
	}
	
	return i;
}

int kb_write(char * buf, uint size)
{
	uint i, t;
	
	for (i = 0, t = key_buf_top; i < size; i++) {		
		t++;
		if (t == KEY_BUF_MAX_SIZE) t = 0;
		if (t == key_buf_bot) break;

		key_buf[key_buf_top] = buf[i];
		key_buf_top = t;
	}

	return i;
}
