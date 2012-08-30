//
// ata.h
//
// written by sjrct
//
// Current limitations of these functions:
//   28 bit lba
//   8 bit sector count
//   assumes master drive in writes to ports
//

#ifndef PIO_H
#define PIO_H

#include "cdef.h"

enum {
	ATA_SUCCESS = 0,
	ATA_ERR_FLOATING_BUS,
	ATA_ERR_NOT_ATA_DEV,
};

//
// ata_identify: Identify and confirms the existence of a drive
//
// Parameters:
//   uchar drive
//
int ata_identify(uchar);

//
// ata_read: Reads from hard disk (primitive)
//
// Parameters:
//   ulong    lba
//   uint     blocks
//   uchar    drive
//   ushort * destination
//
void ata_read(ulong, uint, uchar, ushort *);

//
// ata_read: Writes to hard disk (primitive)
//
// Parameters:
//   ulong    lba
//   uint     blocks
//   uchar    drive
//   ushort * source
//
void ata_write(ulong, uint, uchar, ushort *);

#endif
