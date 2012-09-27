//
// ata.c
//
// written by sjrct
//

#include "ata.h"
#include "kerndef.h"
#include "inline_asm.h"

#define BUSY_BIT      0x80
#define READ_COMMAND  0x20
#define WRITE_COMMAND 0x30

static ata_source active = { 0x1F0, 0, 1 };

static void wait_busy(void)
{
	uchar status;
	do {
		INB(status, active.base_port + 7);
	} while (status & BUSY_BIT);
}

static void ata_common(ulong lba, uint blcks, uchar cmd)
{
	OUTB(active.base_port + 6, 0xE0 | ((lba >> 24) & 0xf) | active.is_slave);
	OUTB(active.base_port + 1, 0);
	OUTB(active.base_port + 2, (uchar)blcks);
	OUTB(active.base_port + 3, (uchar)lba);
	OUTB(active.base_port + 4, (uchar)(lba >> 8));
	OUTB(active.base_port + 5, (uchar)(lba >> 16));

	OUTB(active.base_port + 7, cmd);

	// delay ~400ns
	IO_WAIT;
	IO_WAIT;
	IO_WAIT;
	IO_WAIT;
}

int ata_toggle_source(ata_source * s)
{
	ata_source t = active;
	if (s->valid) {
		active.base_port = s->base_port;
		active.is_slave = !!s->is_slave << 4;
		active.valid = 1;
	}
	*s = t;
	
	return 0;
}

int ata_read(char * data, ulong size, ulong offset)
{
	uint i, j;

	ata_common(offset, size, READ_COMMAND);

	for (i = 0; i < size; i++) {
		wait_busy();
		for (j = 0; j < 0x100; j++)  {
			INW(((ushort*)data)[i * 0x100 + j], active.base_port);
		}
	}

	return 0;
}

int ata_write(char * data, ulong size, ulong offset)
{
	uint i, j;

	ata_common(offset, size, WRITE_COMMAND);

	for (i = 0; i < size; i++) {
		wait_busy();
		for (j = 0; j < 0x100; j++)  {
			OUTW(active.base_port, ((ushort*)data)[i * 0x100 + j]);
		}
	}

	return 0;
}

// Maybe actually use this function ?
/*
static int ata_identify(void)
{
	uchar v1, v2, p1, p2;
	
	// check for floating bus
	INB(v1, active.base_port + 7);
	if (v1 == 0xff) return 0;
	
	// check that is ATA
	INB(p1, active.base_port + 2);
	INB(p2, active.base_port + 3);
	
	OUTB(active.base_port + 2, 0x12); // values are arbitrary
	OUTB(active.base_port + 3, 0x34);
	INB(v1, active.base_port + 2);
	INB(v2, active.base_port + 3);
	
	OUTB(active.base_port + 2, p1);
	OUTB(active.base_port + 3, p2);
	
	if (v1 != 0x12 || v2 != 0x34) return 0;
	
	// identify command
	// TODO
	
	return 1;
}
*/
