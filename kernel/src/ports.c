//
// ports.c
//
// written by sjrct
//

#include "ports.h"
#include "kstrlib.h"
#include "pagemgr.h"
#include "lock.h"

extern kern_obj * current_thread;

static kern_obj * head = NULL;
static int plock = 0;

void send(kern_obj * to, int port, const char * buf, ulong size)
{
	ulong n;
	kern_obj * c;
	kern_obj * l = NULL;
	kern_obj * from, * d;
	
	if (current_thread == NULL) return;
	from = current_thread->u.thrd.proc;
	
	getlock(&plock, 0);
	c = head;
	
	while (c != NULL) {
		if (c->u.porti.port == port && c->u.porti.to == to
			&& c->u.porti.data->u.portd.from == from)
		{
			l = c;
		}
		
		if (c->u.porti.next == NULL) break;
		c = c->u.porti.next;
	}
	
	if (l != NULL) {
		d = l->u.porti.data;
		n = d->u.portd.size + d->u.portd.off;
		
		if (0x1000 - 0x1000 % n >= size) {
			memcpy(d->u.portd.addr + n, buf, size);
			d->u.portd.size += size;
			unlock(&plock, 0);
			return;
		}
	}
	
	n = size / 0x1000;
	if (size % 0x1000) n++;
	
	d = alloc_kobj();
	l = alloc_kobj();
	
	l->type = KOBJ_PORT_INDEX;
	l->u.porti.port = port;
	l->u.porti.data = d;
	l->u.porti.next = NULL;
	l->u.porti.to = to;

	d->type = KOBJ_PORT_DATA;
	d->u.portd.size = size;
	d->u.portd.from = from;
	d->u.portd.addr = (void*)alloc_pages(n, KVIRT_PAGES);
	d->u.portd.off = 0;

	if (c == NULL) {
		head = l;
	} else {
		c->u.porti.next = l;
	}
	
	memcpy(d->u.portd.addr, buf, size);
	
	unlock(&plock, 0);
}

kern_obj * poll(int port)
{
	kern_obj * c;
	kern_obj * to;

	if (current_thread == NULL) return NULL;
	to = current_thread->u.thrd.proc;
	
	getlock(&plock, 0);
	c = head;
	
	while (c != NULL) {
		if (c->u.porti.port == port && c->u.porti.to == to
			&& c->u.porti.data->u.portd.size != 0)
		{
			unlock(&plock, 0);
			return c->u.porti.data->u.portd.from;
		}
		
		c = c->u.porti.next;
	}
	
	unlock(&plock, 0);
	return NULL;
}

int receive(kern_obj * from, int port, char * buf, ulong size)
{
	kern_obj * c;
	kern_obj * to, * d;
	
	if (current_thread == NULL) return 0;
	to = current_thread->u.thrd.proc;
	
	getlock(&plock, 0);
	c = head;
	
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

				unlock(&plock, 0);
				return 1;
			}
		}
		
		c = c->u.porti.next;
	}
	
	unlock(&plock, 0);
	return 0;	
}
