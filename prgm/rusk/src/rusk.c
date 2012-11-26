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

typedef struct full_expanse {
	expanse exp;
	pixel_buffer expanse_buffer;
	pid_t pid;
	char * lbuf;
	bool dirty_lbuf;
	struct full_expanse * next;
} full_expanse;

static full_expanse * expanses;

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
//			resize_buffer(itr->expanse_buffer, itr->exp.width, itr->exp.height);
			itr->lbuf = linear_buffer(itr->expanse_buffer);
			itr->dirty_lbuf = false;
		}
//		itr->exp.x++;
		gr_draw(itr->lbuf, exp->x, exp->y, exp->width, exp->height);
	}
}

int main() {
	reg_esys();
	while(1) {
		serve();
		//poll_keyb();
		//poll_mouse();
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

expanse add_expanse(const api_expanse* e) {
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
	full_expanse* itr;
	
	if(expanses == NULL) {
		return;
	}
	
	for(itr = expanses; itr != NULL; itr = itr->next) {
		if(itr->exp.handle == e.handle) {
			itr->dirty_lbuf = true;
			itr->exp = e;
			break;
		}
	}
}

void bring_expanse_to_front(expanse_handle e) {
	full_expanse* itr = expanses;
	full_expanse* first;
	
	if(itr == NULL || itr->next == NULL) {
		return;
	}
	
	while(itr->next != NULL && itr->next->exp.handle != e) {
		itr = itr->next;
	}
	
	if(itr->next != NULL) {
		first = itr->next;
		itr->next = first->next;
		
		while(itr->next != NULL) {
			itr = itr->next;
		}
		itr->next = first;
	
		first->next = NULL;
	}
}

expanse get_front_expanse() {
	expanse exp;
	if(expanses == NULL) {
		exp.handle = -1;
		return exp;
	}
	return expanses->exp;
}
