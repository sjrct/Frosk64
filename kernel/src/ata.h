//
// ata.h
//
// written by sjrct
//
// Current limitations ata driver:
//   28 bit lba
//   8 bit sector count
//

#ifndef PIO_H
#define PIO_H

#include <cmn/utypes.h>

typedef struct {
	ushort base_port;
	uchar is_slave;
	uchar valid;
} ata_source;

int ata_toggle_source(ata_source *);
int ata_read(char *, ulong, ulong);
int ata_write(char *, ulong, ulong);

#endif
