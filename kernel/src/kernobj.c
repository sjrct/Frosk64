//
// kernobj.c
//
// written by sjrct
//

#include "kernobj.h"

int kernobj_no_new_pages = 1;
static kern_obj * first_hdr;

static const kern_obj blank_hdr = {
	KOBJ_HEADER,
	{.hdr = {
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0 },
		NULL
	}}
};

void init_kobj_mgr()
{
	first_hdr = (kern_obj*)KOBJ_PG0_LOC;
	*first_hdr = blank_hdr;
	first_hdr->u.hdr.next = first_hdr;
}

kern_obj * alloc_kobj()
{
	int b, i;
	kern_obj * c, * f;
	f = c = first_hdr;

	do {
		b = 0;
		if (*c->u.hdr.bitmap != 0xFfffFfffFfffFfff) break;
		b++;
		if (c->u.hdr.bitmap[1] != 0xFfffFfffFfffFfff) break;
		b++;

		c = c->u.hdr.next;
	} while (c != f);

	if (b == 2) return 0;

	for (i = 0; c->u.hdr.bitmap[b] & (1 << i); i++);
	c->u.hdr.bitmap[b] |= 1 << i;

	return (kern_obj*)((char*)c + b * 0x800 + i * 0x20);
}

static kern_obj * loc_kobj(kern_obj * obj, int * b, int * i)
{
	kern_obj * hdr;
	uint off;

	off = (ulong)obj & 0xfff;
	hdr = (kern_obj*)((ulong)obj - off);

	if (off > 0x2000) {
		*b = 1;
		off -= 0x2000;
	} else {
		*b = 0;
	}

	*i = off / 0x20;
	return hdr;
}

void free_kobj(kern_obj * obj)
{
	int i, b;
	kern_obj * hdr = loc_kobj(obj, &b, &i);
	hdr->u.hdr.bitmap[b] &= ~(1 << i);
}

int verify_kobj(kern_obj * obj)
{
	int i, b;
	kern_obj * hdr = loc_kobj(obj, &b, &i);
	return hdr->u.hdr.bitmap[b] & (1 << i);
}
