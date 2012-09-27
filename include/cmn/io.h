//
// io.h
//
// written by sjrct
//

#ifndef _IO_H_
#define _IO_H_

#include <cmn/utypes.h>

typedef struct {
	ulong offset;
	ulong size;
	uchar * data;
} rio_packet;

typedef struct {
	ulong size;
	uchar * data;
} sio_packet;

typedef struct {
	ulong n;
	uchar b;
} unumber;

typedef struct {
	long n;
	uchar b;
} snumber;

#endif
