//
// ata.c
//
// written by sjrct
//

#include "ata.h"
#include "inline_asm.h"
#include "sout.h"

#define BUSY_BIT      0x80

#define READ_COMMAND  0x20
#define WRITE_COMMAND 0x30

static void wait_busy(ushort base_port)
{
	uchar status;
	do {
		INB(status, base_port + 7);
	} while (status & BUSY_BIT);
}

static ushort get_bp(uchar drive)
{
	ushort base_port;

	switch (drive) {
	case 0x00: base_port = 0x3F0; break;
	case 0x01: base_port = 0x370; break;
	case 0x80: base_port = 0x1F0; break;
	case 0x81: base_port = 0x170; break;
	default:
		outs("Error: Drive number ");
		outn(drive, 0x10);
		outs("h is not valid, defaulting to 80h.\n");
		base_port = 0x1F0;
	}
	
	return base_port;
}

static ushort ata_common(ulong lba, uint blcks, uchar drive, uchar cmd)
{
	// Warning: assumes master drive for now
	
	ushort base_port = get_bp(drive);
	
	OUTB(base_port + 6, 0xE0 | ((lba >> 24) & 0xf));
	OUTB(base_port + 1, 0);
	OUTB(base_port + 2, (uchar)blcks);
	OUTB(base_port + 3, (uchar)lba);
	OUTB(base_port + 4, (uchar)(lba >> 8));
	OUTB(base_port + 5, (uchar)(lba >> 16));

	OUTB(base_port + 7, cmd);

	IO_WAIT;
	IO_WAIT;
	IO_WAIT;
	IO_WAIT;
	
	return base_port;
}

int ata_identify(uchar drive)
{
	uchar v1, v2, p1, p2;
	ushort base_port = get_bp(drive);
	
	// check for floating bus
	INB(v1, base_port + 7);
	if (v1 == 0xff) return ATA_ERR_FLOATING_BUS;
	
	// check that is ATA
	INB(p1, base_port + 2);
	INB(p2, base_port + 3);
	
	OUTB(base_port + 2, 0x12); // values are arbitrary
	OUTB(base_port + 3, 0x34);
	INB(v1, base_port + 2);
	INB(v2, base_port + 3);
	
	OUTB(base_port + 2, p1);
	OUTB(base_port + 3, p2);
	
	if (v1 != 0x12 || v2 != 0x34) return ATA_ERR_NOT_ATA_DEV;
	
	// identify command
	
	return ATA_SUCCESS;
}

void ata_read(ulong lba, uint blcks, uchar drive, ushort * dst)
{
	uint i, j;
	ushort base_port;

	base_port = ata_common(lba, blcks, drive, READ_COMMAND);

	for (i = 0; i < blcks; i++) {
		wait_busy(base_port);
		for (j = 0; j < 0x100; j++)  {
			INW(dst[i * 0x200 + j * 2], base_port);
		}
	}
}

void ata_write(ulong lba, uint blcks, uchar drive, ushort * src)
{
	uint i, j;
	ushort base_port;

	base_port = ata_common(lba, blcks, drive, WRITE_COMMAND);
	
	for (i = 0; i < blcks; i++) {
		wait_busy(base_port);
		for (j = 0; j < 0x100; j++)  {
			OUTW(base_port, src[i * 0x200 + j * 2]);
		}
	}
}
