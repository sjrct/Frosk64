//
// rusk.c
//
// written by naitsirhc, sjrct
//

#include <stdlib.h>
#include <stdbool.h>
#include <rusk.h>
#include <bufferutils.h>
#include "rusk_intrn.h"

typedef struct full_window {
	window win;
	pixel_buffer window_buffer;
	pid_t pid;
	window_handle handle;
	char * lbuf;
	bool dirty_lbuf;
	struct full_window * next;
} full_window;

static full_window * windows;

void try_draw() {
	window * win;
	full_window * itr;

	static int counter = 0;
	if (counter++ < 100) return;
	counter = 0;

	for(itr = windows; itr != NULL; itr = itr->next) {
		// TODO change either this or gr draw's parameters
		win = &itr->win;
		if (itr->dirty_lbuf) {
			free(itr->lbuf);
			itr->lbuf = linear_buffer(itr->window_buffer);
			itr->dirty_lbuf = false;
		}
		gr_draw(itr->lbuf, win->x, win->y, win->width, win->height);
	}
}

int main() {
	reg_wsys();

	while(1) {
		serve();
		//poll_keyb();
		//poll_mouse();
		try_draw();
	}
}

/*
void register_wm(pid_t window_manager, int port) {
	wm = window_manager;
	wm_port = port;
}*/

window_handle add_window(const api_window* w) {
	window win;
	pixel pxl = { 0, 128, 0 };
	full_window* itr = windows;
	//TODO THIS MALLOC IS FAILING!!!
	full_window* new_w = malloc(sizeof(full_window));
	
	win.x = 0;
	win.y = 0;
	win.sub_offset_x = 0;
	win.sub_offset_y = 0;
	win.width = w->width;
	win.height = w->height;
	
	win.api_win = *w;

	new_w->next = NULL;
	new_w->win = win;
	new_w->window_buffer = create_buffer(w->width, w->height, pxl);
	new_w->dirty_lbuf = true;
	new_w->lbuf = NULL;

	if (windows == NULL) {
		windows = new_w;
	} else {
		while (itr->next != NULL) {
			itr = itr->next;
		}
		
		itr->next = new_w;
	}
	
	new_w->handle = (window_handle)w;	
	return (window_handle)w;
}

void remove_window(window_handle w) {
	full_window* itr = windows;
	full_window* new_next;
	
	if (windows == NULL) {
		return;
	}
	
	if (itr->next == NULL) {
		if (itr->handle == w) {
			free(windows);
			windows = NULL;
		} // else error?
		return;
	}
	
	while (itr->next != NULL && itr->next->handle != w) {
		itr = itr->next;
	}
	
	if (itr->next != NULL) {
		new_next = itr->next->next;
		free(itr->next);
		itr->next = new_next;
	}
}

/*void bring_window_to_front(window* w) {
	full_window* itr = windows;
	full_window* first;
	
	if(itr == NULL || itr->next == NULL) {
		return;
	}
	
	while(itr->next != NULL && itr->next->w != w) {
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
}*/
