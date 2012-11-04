//
// rusk.c
//
// written by naitsirhc, sjrct
//

#include <stdlib.h>
#include <bufferutils.h>
#include "rusk_intrn.h"

typedef struct full_expanse {
	expanse exp;
	pixel_buffer expanse_buffer;
	pid_t pid;
	expanse_handle handle;
	struct full_expanse * next;
} full_expanse;

static full_expanse * expanses;

void try_draw() {
	expanse * exp;
	full_expanse * itr;
	char * lbuf;

	static int counter = 0;
	if (counter++ < 100) return;
	counter = 0;
	
	if (expanses != NULL) {
		for(itr = expanses; itr->next != NULL; itr = itr->next) {
			exp = &itr->exp;
			lbuf = linear_buffer(itr->expanse_buffer);
			gr_draw(lbuf, exp->x, exp->y, exp->width, exp->height);
		}
	}
}

int main() {
	reg_esys();

	while(1) {
		serve();
	
		//poll_keyb();
		//poll_mouse();
		//try_draw();
	}
}

/*void register_em(pid_t expanse_manager, int port) {
	em = expanse_manager;
	em_port = port;
}*/

expanse_handle add_expanse(const api_expanse* e) {
	expanse exp;
	pixel pxl = { 0, 128, 0 };
	full_expanse* itr = expanses;
	full_expanse* new_e = malloc(sizeof(full_expanse));

	exp.x = 0;
	exp.y = 0;
	exp.sub_offset_x = 0;
	exp.sub_offset_y = 0;
	exp.width = e->width;
	exp.height = e->height;
	
	exp.api_exp = *e;
	
	new_e->next = NULL;
	new_e->exp = exp;
	new_e->expanse_buffer = create_buffer(e->width, e->height, pxl);
	
	if (itr == NULL) {
		expanses = new_e;
	} else {
		while (itr->next != NULL) {
			itr = itr->next;
		}
		
		new_e->next = itr->next;
		itr->next = new_e;
	}
	
	new_e->handle = (expanse_handle)e;
	return new_e->handle;
}

void remove_expanse(expanse_handle e) {
	full_expanse* itr = expanses;
	full_expanse* new_next;
	
	if (expanses == NULL) {
		return;
	}
	
	if (itr->next == NULL) {
		if (itr->handle == e) {
			free(expanses);
			expanses = NULL;
		} // else error?
		return;
	}
	
	while (itr->next != NULL && itr->next->handle != e) {
		itr = itr->next;
	}
	
	if (itr->next != NULL) {
		new_next = itr->next->next;
		free(itr->next);
		itr->next = new_next;
	}
}

void bring_expanse_to_front(expanse_handle e) {
	full_expanse* itr = expanses;
	full_expanse* first;
	
	if(itr == NULL || itr->next == NULL) {
		return;
	}
	
	while(itr->next != NULL && itr->next->handle != e) {
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