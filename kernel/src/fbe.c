//
// fbe.c
//
// written by sjrct
//

#include "fbe.h"

#define FBE_MARK 0xFBE0FBE0

kern_obj * exec_fbe(const uchar * buf, ulong buf_sz, kern_obj * par, uchar priv, uchar prior)
{
	uint csz, dsz, bsz;

	if (ATD(buf) != FBE_MARK) return NULL;

	csz = ATD(buf + 0x4);
	dsz = ATD(buf + 0x8);
	bsz = ATD(buf + 0xC);

	if (csz > buf_sz) csz = buf_sz;
	if (dsz > buf_sz - csz) dsz = buf_sz - csz;

	return spawn_process(buf + 0x10, csz, dsz, bsz, par, priv, prior);
}
