//
// rusk_comm.c
//
// written by naitsirhc
//

#include <frosk.h>
#include <stdlib.h>
#include <bufferutils.h>
#include "rusk_intrn.h"

void debug();

static void create_expanse_func(pid_t id) {
	expanse_handle handle = 0;
	api_expanse * exp = malloc(sizeof(api_expanse));

	while (!receive(id, ES_COMM_PORT, exp, sizeof(api_expanse)));
	handle = add_expanse(exp);
	
	//TODO Tell EM to format expanse
	
	// send expanse_handle to process id
	send(id, ES_COMM_PORT, &handle, sizeof(handle));
}

static void remove_expanse_func(pid_t id) {
	expanse_handle * handle;

	while (!receive(id, ES_COMM_PORT, &handle, sizeof(handle)));
	remove_expanse(*handle);
}

void debug(int i) {
	pixel pxl = { 100, 100, 0 };
	pixel_buffer p = create_buffer(10, 10, pxl);
	gr_draw(linear_buffer(p), 400, i * 10, 10, 10);
}

static void update_partial_expanse_func(pid_t id) {
	pixel_buffer p;
	int x, y;
	expanse_handle handle;
	while(!receive(id, ES_COMM_PORT, &handle, sizeof(expanse_handle)));
	while(!receive(id, ES_COMM_PORT, &x, sizeof(int)));
	while(!receive(id, ES_COMM_PORT, &y, sizeof(int)));
	p = receive_buffer(id, ES_COMM_PORT);
	update_partial(handle, p, x, y);
//	gr_draw(linear_buffer(p), 300, 100, p.width, p.height);
	debug(5);
}

// From expanse manager
/*static void update_expanse_func(pid_t id) {
	expanse e = read_expanse();
	full_expanse exp = find_expanse(e);
	exp.exp = e;
}

static void update_expanse_border_func(pid_t id) {
	void * exp = read_pointer();
	pixel * buffer = read_buffer();
	// fun stuff to copy peices of buffer to border segments
}*/


static void (*funcs[])(pid_t) = {
	create_expanse_func,
	remove_expanse_func,
	update_partial_expanse_func,
/*	update_decoration_buffer_func,
	update_expanse_func,
	,*/
};

void serve(void) {
	pid_t id;
	int func;
	
	while ((id = poll(ES_COMM_PORT))) {
		receive(id, ES_COMM_PORT, &func, sizeof(int));
		debug(func);
		funcs[func](id);
	}
}
