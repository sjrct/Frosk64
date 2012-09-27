//
// fs.c
//
// written by sjrct
//

#include <stdbool.h>
#include "fs.h"
#include "kernobj.h"
#include "kstrlib.h"
#include "ata.h"
#include "driver.h"
#include "ehci.h"
#include "lock.h"

#define LOCK_BIT 7

#define GET_TYPE(h) (h->type & 0xf)

#pragma pack(push, 1)
typedef struct {
	uint mark;
	uint root;
	uint predir;
	uint unused;
	ulong next_lba;
} fs_header;
#pragma pack(pop)

static kern_obj * first_hdr;
static int use_ata;

void init_fs(void)
{
	ata_source src;
	ehci_pci_inf pi;

	first_hdr = alloc_kobj();
	first_hdr->type = KOBJ_FSH_META;
	first_hdr->u.fshm.addr = FST0_LOC + KSPACE_LOC;
	first_hdr->u.fshm.next = NULL;
	
	use_ata = !ATB(DISK_INF_LOC + 3);

	if (use_ata) {
		src.base_port = ATW(DISK_INF_LOC);
		src.is_slave = ATB(DISK_INF_LOC + 2);
//		driver_call(DRIVER_ID_ATA, DRIVER_FUNC_SOURCE, &src);
		ata_toggle_source(&src);
	} else {
		pi.bus = ATB(DISK_INF_LOC);
		pi.dev = ATB(DISK_INF_LOC + 1);
		pi.fun = ATB(DISK_INF_LOC + 2);
		driver_call(DRIVER_ID_EHCI, DRIVER_FUNC_SETUP, &pi);
	}
}

static handle_t resolve(uint ptr)
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
	
	return (handle_t)(koh->u.fshm.addr + (ulong)entry);
}

handle_t fs_aquire(const char * fn, handle_t cd, int res)
{
	bool good;
	int i;
	handle_t h, n;
	
	if (*fn == '/') {
		h = resolve( ((fs_header*)first_hdr->u.fshm.addr)->root );
		fn++;
	} else {
		h = cd;
		if (h == 0) return 0;
	}
	
	// TODO verify cd/pd

	if (*fn == '\0') return h;
	if (GET_TYPE(h) != FS_DIRECTORY) return 0;

	if (h->u.dir.first_file_ptr) {
		h = resolve(h->u.dir.first_file_ptr);
		while (1) {
			n = resolve(h->u.file.name_ptr);
		
			if (!strncmp(n->u.string.str, fn, n->u.string.size)) {
				good = true;
				i = 0;
			
				while (n->u.string.next_ptr != 0) {
					i += n->u.string.size;
					n = resolve(n->u.string.next_ptr);

					if (strncmp(n->u.string.str, fn + i, n->u.string.size)) {
						good = false;
						break;
					}
				}

				i += n->u.string.size;
			
				if (good) {
					if (fn[i] == '/') {
						if (GET_TYPE(h) != FS_DIRECTORY) return 0;
						return fs_aquire(fn + i + 1, h, res);
					} else if (fn[i] == '\0') {
						if (!trylock(&h->type, LOCK_BIT)) return 0;
						return h;
					}
				}
			}
		
			if (h->u.file.next_ptr == 0) break;
			h = resolve(h->u.file.next_ptr);
		}
	}
	
	return 0;
}

int fs_release(handle_t h)
{
	// TODO verify
	return unlock(&h->type, LOCK_BIT);
}

ulong fs_read(handle_t h, char * buf, ulong bl_max, ulong bl_offset)
{
	ulong sz, off, obm = bl_max;
	
	// TODO verify

	if (GET_TYPE(h) == FS_FILE) {
		h = resolve(h->u.file.first_data_ptr);
		
		while (bl_max > 0) {
			sz = h->u.data.blocks;
			if (sz < bl_offset) {
				bl_offset -= sz;
			} else {
				sz = bl_max > sz ? sz : bl_max;
				off = h->u.data.lba + bl_offset;
				
				if (use_ata) {
					ata_read(buf, sz * SECTS_PER_BLOCK, off * SECTS_PER_BLOCK);  
				} else {
					// TODO
				}

				bl_max -= sz;
				buf += sz * 0x1000;
				bl_offset -= off;
			}
			
			if (h->u.data.next_ptr == 0) break;
			h = resolve(h->u.data.next_ptr);
		}
	}
	
	return obm - bl_max;
}

ulong fs_write(handle_t, const char *, ulong, ulong);

ulong fs_get_size(handle_t h)
{
	ulong sz = 0;

	// TODO verify
	
	if (GET_TYPE(h) == FS_FILE && h->u.file.first_data_ptr) {
		h = resolve(h->u.file.first_data_ptr);
		while (1) {
			sz += (h->u.data.blocks - 1) * 0x1000 + h->u.data.bytes_in_last_block;
			if (h->u.data.next_ptr == 0) break;
			h = resolve(h->u.data.next_ptr);
		}
	}
	
	return sz;
}

int fs_get_filename(handle_t, char *, ulong);
ulong fs_get_perm(handle_t);
int fs_set_perm(handle_t, ulong);
handle_t fs_create_file(const char *, handle_t, handle_t);
handle_t fs_create_dir(const char *, handle_t, handle_t);
handle_t fs_create_indirect(const char *, const char *, handle_t, handle_t);
int fs_remove(handle_t);
