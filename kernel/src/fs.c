//
// fs.c
//
// written by sjrct
//

#include "fs.h"
#include "kernobj.h"
#include "kstrlib.h"
#include "sout.h"

#pragma pack(push, 1)
typedef struct {
	uint mark;
	uint root;
	uint predir;
	uint unused;
	ulong next_lba;
} fs_header;
#pragma pack(pop)

uchar fs_drive;

static kern_obj * first_hdr;

void init_fs()
{
	first_hdr = alloc_kobj();
	first_hdr->type = KOBJ_FSH_META;
	first_hdr->u.fshm.addr = FST0_LOC + KSPACE_LOC;
	first_hdr->u.fshm.next = NULL;

	fs_drive = ATB(DRIVE_LOC);
}

fs_node * fs_resolve_ptr(uint ptr)
{
	uint entry, hdri;
	kern_obj * koh;
	
	entry = (ptr & 0xff) * 24;
	hdri = ptr >> 8;
	koh = first_hdr;
	
	for (; hdri; hdri--) {
		if (koh->u.fshm.next == NULL) {
			// TODO
		}
		koh = koh->u.fshm.next;
	}
	
	return (fs_node*)(koh->u.fshm.addr + (ulong)entry);
}

fs_node * fs_locate_node(const char * fn, fs_node * cd)
{
	fs_node * n, * s;

	if (*fn == '/') {
		fn++;
		n = fs_resolve_ptr( ((fs_header*)first_hdr->u.fshm.addr)->root );
		if (*fn == '\0') return n;
	} else {
		n = cd;
	}

	n = fs_resolve_ptr( n->u.dir.first_file_ptr );
	while (n != 0) {
		s = fs_resolve_ptr(n->u.file.name_ptr);
		
		// TODO support more than 1 string node for names
		if (!memcmp(s->u.string.str, fn, s->u.string.size)) {
			if (fn[s->u.string.size] == '/') {
				// TODO check for not being a directory
				return fs_locate_node(fn + s->u.string.size + 1, n);
			} else {
				return n;
			}
		}
		
		n = fs_resolve_ptr(n->u.file.next_ptr);
	}
	
	return NULL;
}
