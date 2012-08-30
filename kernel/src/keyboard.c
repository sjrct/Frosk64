//
// keyboard.c
//
// written by sjrct
//

#include "keyboard.h"
#include "kerndef.h"
#include "interrupt.h"
#include "irq.h"

char key_buf[KEY_BUF_MAX_SIZE];
uint key_buf_size;

static char ascii_map[] = {
	000, 000, '1', '2', '3',
	'4', '5', '6', '7', '8',
	'9', '0', '-', '=',
	// TODO
};

void init_keyboard()
{
	void key_int();
	register_int(IRQ0 + IRQ_KEYBOARD, (ulong)key_int, KERNEL_CS, 0x8E);
	enable_irq(IRQ_KEYBOARD);
}

int getsc()
{
	if (key_buf_size == 0) return -1;
	return key_buf[--key_buf_size];
}

int sc_to_ascii(int sc)
{
	if (sc < 0 || sc >= sizeof(ascii_map)) return 0;
	return ascii_map[sc];
}
