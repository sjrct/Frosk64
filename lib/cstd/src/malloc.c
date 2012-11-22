//
// malloc.c
//
// written by sjrct
//

#include <stdlib.h>
#include <stdbool.h>
#include <debug.h>

typedef struct header {
	size_t size;
	struct header * next;
} header;

extern header _heap_loc;
static header * hdr = NULL;

void * malloc(size_t sz)
{
//	debug_line("**");
//	debug_number(sz);

	static bool first = true;
	header * cur, * prv;

	if (first) {
		hdr = &_heap_loc;
		hdr->size = 0x100000; // FIXME size fudged for now		
		hdr->next = hdr;
		first = false;
	}


	if (hdr == NULL) return NULL;
	
	if (sz % sizeof(header) != 0) {
		sz += sizeof(header) - (sz % sizeof(header));
	}
	
	cur = hdr->next;
	prv = hdr;
	do {
		if (cur->size >= sz) {
//			debug_line("***");
//			debug_number(cur+1);

			if (cur->size == sz) {
				if (cur == cur->next) {
					hdr = NULL;
				} else {
					prv->next = cur->next;
					hdr = prv;
				}
				return cur + 1;
			} else {
				hdr = (header*)((char*)(cur + 1) + sz);
				hdr->size = cur->size - sz - sizeof(header);
				cur->size = sz;
				
				if (prv != cur) {
					hdr->next = cur->next;
					prv->next = hdr;
				} else {
					hdr->next = hdr;
				}
				
				return cur + 1;
			}
		}
		
		prv = cur;
		cur = cur->next;
	} while (cur == hdr->next);
	debug_line("MALLOC FAIL!");
	debug_number(cur);
	debug_number(hdr->next);
	return NULL;
}

void * calloc(size_t cnt, size_t sz)
{
	size_t i;
	char * d;
	
	d = malloc(cnt * sz);
	if (d == NULL) return NULL;
	for (i = 0; i < cnt * sz; i++) d[i] = 0;
	
	return d;
}

void * realloc(void * old, size_t sz)
{
	size_t i;
	char * nw = malloc(sz);
	header * oldh = (header*)old - 1;
	
	for (i = 0; i < sz; i++) {
		if (i >= oldh->size) break;
		nw[i] = ((char*)old)[i];
	}
	
	for (; i < sz; i++) nw[i] = 0;
	
	free(old);
	return nw;
}

void free(void * ptr)
{
	return;
	debug_line("free: ");
	debug_number(ptr);
	header * cur, * prv;
	header * ph;
	
	if (ptr == NULL) return;
	
	ph = (header*)ptr - 1;
	
	if (hdr == NULL) {
		hdr = ph;
		hdr->next = hdr;
	} else {
		prv = hdr;
		cur = hdr->next;
		
		do {
			if ((char*)(cur + 1) + cur->size == (char*)ptr) {
				cur->size += ph->size + sizeof(header);
				return;
			}
			
			if ((char*)ptr + ph->size == (char*)cur) {
				ph->size += cur->size + sizeof(header);
				if (prv == cur) {
					hdr = ph;
					hdr->next = hdr;
				} else {
					prv->next = ph;
					ph->next = cur->next;
				}
				return;
			}
			
			prv = cur;
			cur = cur->next;
		} while (cur != hdr->next);
	
		ph->next = hdr->next;
		hdr->next = ph;
	}
	debug_line("freedone");
}
