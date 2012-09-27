//
// fdrvr.h
//
// written by sjrct
//

#ifndef _FDRVR_H_
#define _FDRVR_H_

// driver types
enum {
	DRIVER_OTHER = 0,
	DRIVER_SCHAR,
	DRIVER_SBLOCK,
	DRIVER_RCHAR,
	DRIVER_RBLOCK,
	DRIVER_MOUSE,
};

// driver id numbers
enum {
	DRIVER_ID_ATA = 0,
	DRIVER_ID_EHCI,
	DRIVER_ID_KEYBOARD,
	DRIVER_ID_CGA_TEXT,
};

// driver function numbers
// all driver types
#define DRIVER_FUNC_EXISTS -1
#define DRIVER_FUNC_INIT   0

// for drivers of type [s/r][char/block]
#define DRIVER_FUNC_READ   1
#define DRIVER_FUNC_WRITE  2
#define DRIVER_FUNC_SOURCE 3

// for text-mode graphics drivers (type=other)
#define DRIVER_FUNC_TEXT_CLS   1
#define DRIVER_FUNC_TEXT_PUTC  2
#define DRIVER_FUNC_TEXT_PUTS  3
#define DRIVER_FUNC_TEXT_PUTU  4
#define DRIVER_FUNC_TEXT_PUTL  5
#define DRIVER_FUNC_TEXT_PUTNL 6

// other driver functions
#define DRIVER_FUNC_SETUP 1

#endif
