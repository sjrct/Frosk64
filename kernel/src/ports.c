//
// ports.c
//
// written by sjrct
//

#include "ports.h"
#include "kstrlib.h"
#include "pagemgr.h"

extern kern_obj * current_thread;

static kern_obj * head = NULL;

void send(kern_obj * to, int port, const char * buf, ulong size)
{
	ulong n;
	kern_obj * c = head;
	kern_obj * l = NULL;
	kern_obj * from, * d;

	if (current_thread == NULL) return;
	from = current_thread->u.thrd.proc;
	
	while (c != NULL) {
		if (c->u.porti.port == port && c->u.porti.to == to
			&& c->u.porti.data->u.portd.from == from)
		{
			l = c;
		}
		
		c = c->u.porti.next;
	}
	
	if (l != NULL) {
		d = l->u.porti.data;
		n = d->u.portd.size + d->u.portd.off;
		
		if (0x1000 - 0x1000 % n >= size) {
			memcpy(d->u.portd.addr + n, buf, size);
			d->u.portd.size += size;
			return;
		}
	}
	
	n = size / 0x1000;
	if (size % 0x1000) n++;
	
	d = alloc_kobj();
	c = alloc_kobj();
	
	c->type = KOBJ_PORT_INDEX;
	c->u.porti.port = port;
	c->u.porti.data = d;
	c->u.porti.next = head;
	c->u.porti.to = to;

	d->type = KOBJ_PORT_DATA;
	d->u.portd.size = size;
	d->u.portd.from = from;
	d->u.portd.addr = (void*)alloc_pages(n, KVIRT_PAGES);
	d->u.portd.off = 0;
	
	head = c;
	memcpy(d->u.portd.addr, buf, size);
}

kern_obj * poll(int port)
{
	kern_obj * c = head;
	kern_obj * to;

	if (current_thread == NULL) return NULL;
	to = current_thread->u.thrd.proc;
	
	while (c != NULL) {
		if (c->u.porti.port == port && c->u.porti.to == to) {
			return c->u.porti.data->u.portd.from;
		}
		
		c = c->u.porti.next;
	}
	
	return NULL;
}

int receive(kern_obj * from, int port, char * buf, ulong size)
{
	kern_obj * c = head;
	kern_obj * to, * d;
	
	if (current_thread == NULL) return 0;
	to = current_thread->u.thrd.proc;
	
	while (c != NULL) {
		if (c->u.porti.port == port && c->u.porti.to == to) {
			d = c->u.porti.data;
			
			if (d->u.portd.from == from && d->u.portd.size >= size) {
				memcpy(buf, d->u.portd.addr + d->u.portd.off, size);
				
				d->u.portd.size -= size;
				d->u.portd.off += size;

				if (d->u.portd.off >= 0x1000) {
					free_pages((ulong)d->u.portd.addr, d->u.portd.off / 0x1000, KVIRT_PAGES);
					d->u.portd.off %= 0x1000;
				}

				return 1;
			}
		}
		
		c = c->u.porti.next;
	}

	return 0;	
}
