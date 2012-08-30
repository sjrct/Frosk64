//
// cdef.h
//
// written by sjrct
//

#ifndef CDEF_H
#define CDEF_H

#define NULL 0

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

#define ATB(X) (*((uchar *) (X)))
#define ATW(X) (*((ushort *)(X)))
#define ATD(X) (*((uint *)  (X)))
#define ATQ(X) (*((ulong *) (X)))

#endif
