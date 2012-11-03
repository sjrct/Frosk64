//
// rusk_comm.c
//
// written by naitsirhc
//

#include <frosk.h>
#include <rusk.h>
#include <stdlib.h>
#include "rusk_intrn.h"

// temporary
#define SHINY_REMOVE_WINDOW 1
#define BEARING_REMOVE_WINDOW 1

static void create_window_func(pid_t id) {
	window_handle handle;
	api_window * win = malloc(sizeof(win));

	while (!receive(id, RUSK_COMM_PORT, win, sizeof(api_window)));
	handle = add_window(win);
	
	//TODO Tell WM to format window
	
	// send window_handle to process id
	send(id, RUSK_COMM_PORT, &handle, sizeof(handle));
}

static void remove_window_func(pid_t id) {
	int n;
	window_handle * handle;

	while (!receive(id, RUSK_COMM_PORT, &handle, sizeof(handle)));
	remove_window(*handle);
	
	//TODO tell WM about removed window
	//n = BEARING_REMOVE_WINDOW;
	//send(wm, wm_port, &n, sizeof(int));
	//send(wm, wm_port, &handle, sizeof(handle));
	
	n = SHINY_REMOVE_WINDOW;
	send(id, RUSK_COMM_PORT, &n, sizeof(n));
	send(id, RUSK_COMM_PORT, &handle, sizeof(handle));
}

// From window manager
/*static void update_window_func(pid_t id) {
	window w = read_window();
	full_window win = find_window(w);
	win.win = w;
}

static void update_window_border_func(pid_t id) {
	void * win = read_pointer();
	pixel * buffer = read_buffer();
	// fun stuff to copy peices of buffer to border segments
}*/


static void (*funcs[])(pid_t) = {
	create_window_func,
	remove_window_func,
/*	update_decoration_buffer_func,
	update_window_func,
	update_partial_window_func,*/
};

void serve(void) {
	pid_t id;
	int func;
	
	while ((id = poll(RUSK_COMM_PORT))) {
		receive(id, RUSK_COMM_PORT, &func, sizeof(int));
		funcs[func](id);
	}
}
