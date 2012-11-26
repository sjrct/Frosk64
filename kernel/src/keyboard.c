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
			key_buf_top--;
			if (key_buf_top == -1) key_buf_top = KEY_BUF_MAX_SIZE - 1;
			buf[i] = key_buf[key_buf_top];
		} else {
			break;
		}
	}
	
	if (i == 0) return 0;
	else return key_buf_top + 1;
	return i;
}

int kb_write(char * buf, uint size)
{
	uint i, t;
	
	for (i = 0, t = key_buf_bot; i < size; i++) {
		t++;
		if (t == KEY_BUF_MAX_SIZE) t = 0;
		
		if (t == key_buf_top) break;
		
		key_buf[t] = buf[i];
		key_buf_bot = t;
	}

	return size - i;
}
