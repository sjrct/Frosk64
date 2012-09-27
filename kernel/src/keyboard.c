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

static int kb_init(void);
static int kb_read(sio_packet *);
static int kb_write(sio_packet *);

int (*keyboard_functions[KEYBOARD_FUNC_COUNT])() = {
	kb_init,
	kb_read,
	kb_write
};

char key_buf[KEY_BUF_MAX_SIZE];
uint key_buf_top = 0;
uint key_buf_bot = 0;

static int kb_init(void)
{
	void key_int();
	register_int(IRQ0 + IRQ_KEYBOARD, (ulong)key_int, KERNEL_CS, 0x8F);
	enable_irq(IRQ_KEYBOARD);
	return 0;
}

static int kb_read(sio_packet * p)
{
	uint i, r;

	for (i = r = 0; i < p->size; i++) {
		if (key_buf_top != key_buf_bot) {
			key_buf_top--;
			if (key_buf_top == -1) key_buf_top = KEY_BUF_MAX_SIZE - 1;
			p->data[i] = key_buf[key_buf_top];
		} else {
			p->data[i] = 0xFF;
			r++;
		}
	}
	
	return r;
}

static int kb_write(sio_packet * p)
{
	uint i, t;
	
	for (i = 0, t = key_buf_bot; i < p->size; i++) {
		t++;
		if (t == KEY_BUF_MAX_SIZE) t = 0;
		
		if (t == key_buf_top) break;
		
		key_buf[t] = p->data[i];
		key_buf_bot = t;
	}

	return p->size - i;
}
