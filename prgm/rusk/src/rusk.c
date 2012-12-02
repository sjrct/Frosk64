//
// rusk.c
//
// written by naitsirhc, sjrct
//

#include <stdlib.h>
#include <stdbool.h>
#include <expanse.h>
#include <bufferutils.h>
#include "rusk_intrn.h"
#include <debug.h>

static full_expanse * expanses;
static int mx;
static int my;
void draw_mouse(int x, int y);

void try_draw() {
	expanse * exp;
	full_expanse * itr;
	
	for(itr = expanses; itr != NULL; itr = itr->next) {
		exp = &itr->exp;
		if(!exp->visible) continue;
		if (itr->dirty_lbuf) {
			if(itr->lbuf != NULL) {
				free(itr->lbuf);
			}
//			resize_buffer(&itr->expanse_buffer, itr->exp.width, itr->exp.height);
			itr->lbuf = linear_buffer(itr->expanse_buffer);
			itr->dirty_lbuf = false;
		}
		gr_draw(itr->lbuf, exp->x, exp->y, exp->width, exp->height);
	}
	draw_mouse(mx,my);
}
void draw_mouse(int x, int y) {
	static int px = -1, py = -1;
	pixel p = { 255, 255, 255};
	pixel b = { 0, 0, 0};
	
	if (px != -1 && py != -1 && ( px != x || py != y)) {
		gr_fill(&b, px-5, py, 10, 1);
		gr_fill(&b, px, py-5, 1, 10);		
	}
	
	gr_fill(&p, x-5, y, 10, 1);
	gr_fill(&p, x, y-5, 1, 10);
	
	px = x;
	py = y;
}
void print_event(event e) {
	write_serial('*');
	write_serial(e.u.keyboard.letter);
	write_serial('\n');
//	debug_number(e.u.keyboard.shift);
}

void add_event(event_list ** list, event_list ** itr, event event) {	
	if(*list == NULL) {
		*list = malloc(sizeof(event_list));
		*itr = *list;
	} else {
		(*itr)->next = malloc(sizeof(event_list));
		*itr = (*itr)->next;
	}
	(*itr)->event = event;
}

#define SHIFT 0x2a
#define CTRL 0x1d
#define ALT 0x38
#define UP 0x80
bool events_allowed = true;
void process_events(){
	uchar c[64];
	static char keys[0x36] = "\n\n1234567890-=\n\tqwertyuiop[]\n_asdfghjkl;'`\\_zxcvbnm,./";
	static bool shift;
	static bool ctrl;
	static bool alt;
	int count, i;
	event event;
	
	if (!events_allowed) return;
	
	event_list * list = NULL;
	event_list * itr = NULL;
	
	update_mouse(&mx,&my);
	count = kb_read(c,64);
	
	if(mx < 0) mx=0;
	if(my < 0) my=0;
	mx+=10;
	my+=10;
	for(i = 0; i < count; i++) {
		switch(c[i]) {
			case SHIFT:
				shift = true;
				continue;
			case SHIFT + UP:
				shift = false;
				continue;
			case CTRL:
				ctrl = true;
				continue;
			case CTRL + UP:
				ctrl = true;
				continue;
			case ALT:
				alt = true;
				continue;
			case ALT + UP:
				alt = true;
				continue;
			case LEFT_MOUSE:
				event.u.mouse.x = mx;
				event.u.mouse.y = my;
				event.u.mouse.button = 1;
				event.type = MOUSE_DOWN;
				add_event(&list, &itr, event);
				continue;
			case RIGHT_MOUSE:
				event.u.mouse.x = mx;
				event.u.mouse.y = my;
				event.u.mouse.button = 2;
				event.type = MOUSE_DOWN;
				add_event(&list, &itr, event);
				continue;
			case MIDDLE_MOUSE:
				event.u.mouse.x = mx;
				event.u.mouse.y = my;
				event.u.mouse.button = 3;
				event.type = MOUSE_DOWN;
				add_event(&list, &itr, event);
				continue;
			case LEFT_MOUSE + UP:
				event.u.mouse.x = mx;
				event.u.mouse.y = my;
				event.u.mouse.button = 1;
				event.type = MOUSE_UP;
				add_event(&list, &itr, event);
				continue;
			case RIGHT_MOUSE + UP:
				event.u.mouse.x = mx;
				event.u.mouse.y = my;
				event.u.mouse.button = 2;
				event.type = MOUSE_UP;
				add_event(&list, &itr, event);
				continue;
			case MIDDLE_MOUSE + UP:
				event.u.mouse.x = mx;
				event.u.mouse.y = my;
				event.u.mouse.button = 3;
				event.type = MOUSE_UP;
				add_event(&list, &itr, event);
				continue;
		}
		if(c[i] <= 0x35 || c[i] == 0x39) {
			if(c[i] == 0x39) {
				event.u.keyboard.letter = ' ';
			} else {		
				event.u.keyboard.letter = keys[c[i]];
			}
			event.type = KEY_DOWN;
			event.u.keyboard.shift = shift;
			event.u.keyboard.ctrl = ctrl;
			event.u.keyboard.alt = alt;
			add_event(&list, &itr, event);
//			print_event(event);
		}
	}
	
	if(list != NULL) {
		handle_events(list);
	}
	free_event_list(list);
//	draw_mouse(mx,my);
}

int main() {
	reg_esys();
	
	while(1) {
		serve();
		process_events();
		try_draw();
	}
}

void update_partial(expanse_handle handle, pixel_buffer p, int x, int y) {
	int i, j;
	
	full_expanse * itr;
	for(itr = expanses; itr != NULL && itr->exp.handle != handle; itr = itr->next);
	if(itr == NULL) return;
	for(i = 0; i < p.width; i++) {
		for(j = 0; j < p.height; j++) {
			itr->expanse_buffer.buffer[i+x][j+y] = p.buffer[i][j];
		}
	}
	itr->dirty_lbuf = true;
}

void em_registered() {
	full_expanse * itr;
	for(itr = expanses; itr != NULL; itr = itr->next) {
		em_init_expanse(itr->exp);
	}
}

expanse add_expanse(const api_expanse* e, pid_t pid) {
	expanse exp;
	pixel pxl = { 0, 128, 0 };

	full_expanse* itr = expanses;
	full_expanse* new_e = malloc(sizeof(full_expanse));

	exp.x = 10;
	exp.y = 10;
	exp.sub_offset_x = 0;
	exp.sub_offset_y = 0;
	exp.width = e->width;
	exp.height = e->height;
	exp.visible = true;
	
	exp.api_exp = *e;
	
	new_e->next = NULL;
	new_e->exp = exp;
	new_e->expanse_buffer = create_buffer(e->width, e->height, pxl);
	new_e->dirty_lbuf = true;
	new_e->lbuf = NULL;	
	new_e->exp.handle = (expanse_handle)e;
	new_e->pid = pid;
	
	if (itr == NULL) {
		expanses = new_e;
	} else {
		while (itr->next != NULL) {
			itr = itr->next;
		}
		
		itr->next = new_e;
	}
	
	return new_e->exp;
}

void remove_expanse(expanse_handle e) {
	full_expanse* itr = expanses;
	full_expanse* new_next;
	
	if (expanses == NULL) {
		return;
	}
	
	if (itr->next == NULL) {
		if (itr->exp.handle == e) {
			free(expanses);
			expanses = NULL;
		} // else error?
		return;
	}
	
	while (itr->next != NULL && itr->next->exp.handle != e) {
		itr = itr->next;
	}
	
	if (itr->next != NULL) {
		new_next = itr->next->next;
		free(itr->next);
		itr->next = new_next;
	}
}

void update_expanse(expanse e) {
	pixel p = { 0, 0, 0 };
	full_expanse* itr;
	
	if(expanses == NULL) {
		return;
	}
	
	for(itr = expanses; itr != NULL; itr = itr->next) {
		if(itr->exp.handle == e.handle) {
			itr->dirty_lbuf = true;
			if(itr->exp.x != e.x || itr->exp.y != e.y) {
				gr_fill(&p, (itr->exp.x), itr->exp.y, itr->exp.width, itr->exp.height);
			}
			itr->exp = e;
			break;
		}
	}
}

void bring_expanse_to_front(expanse_handle e) {
	full_expanse* itr;
	full_expanse* first;
	
	if(expanses == NULL || expanses->next == NULL) {
		return;
	}
	
	for(itr = expanses; itr->next != NULL && itr->next->exp.handle != e; itr = itr->next);
	
	if(itr->next != NULL) {
		first = itr->next;
		for(itr->next = first->next; itr->next != NULL; itr = itr->next);
		itr->next = first;
		first->next = NULL;
	}
}

full_expanse * get_front_expanse() {
	return expanses;
}
void adjust_events(event_list* elist) {
	full_expanse * fexp = expanses;
	
	for(; elist != NULL; elist = elist->next) {
		if(elist->event.type == MOUSE_DOWN || elist->event.type == MOUSE_UP) {
			//TODO account for border
			elist->event.u.mouse.x -= fexp->exp.x;
			elist->event.u.mouse.y -= fexp->exp.y;
		}
	}
}
