//
// malloc.c
//
// written by sjrct
//

#include <stdlib.h>
#include <stdbool.h>

typedef struct header {
	size_t size;
	struct header * next;
} header;

extern void * const _heap_offset;
static header * hdr = NULL;

void * malloc(size_t sz)
{
	static bool first = true;
	header * cur, * prv;
	
	if (first) {
		hdr = _heap_offset;
		hdr->size = 0x10000; // FIXME size fudged for now
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
			if (cur->size == sz) {
				if (cur == cur->next) {
					hdr = NULL;
				} else {
					prv->next = cur->next;
					hdr = prv;
				}
				return cur + 1;
			} else {
				hdr = (header*)((char*)cur + sz);
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
	header * cur, * prv;
	header * ph = (header*)ptr - 1;
	
	if (hdr == NULL) {
		hdr = ph;
		hdr->next = NULL;
	} else {
		prv = hdr;
		cur = hdr->next;
		
		do {
			if ((char*)cur + cur->size + sizeof(header) == (char*)ph) {
				cur->size += ph->size;
				return;
			}
			
			if ((char*)ph + ((header*)ph)->size + sizeof(header) == (char*)cur) {
				ph->size += cur->size;
				if (prv == cur) hdr = cur;
				else prv->next = ph;
				return;
			}
			
			prv = cur;
			cur = cur->next;
		} while (cur != hdr->next);
	
		ph->next = hdr->next;
		hdr->next = ph;
	}
}
