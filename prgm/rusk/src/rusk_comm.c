//
// rusk_comm.c
//
// written by naitsirhc
//

#include <frosk.h>
#include <stdlib.h>
#include "rusk_intrn.h"

static void create_expanse_func(pid_t id) {
	expanse_handle handle;
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
/*	update_decoration_buffer_func,
	update_expanse_func,
	update_partial_expanse_func,*/
};

void serve(void) {
	pid_t id;
	int func;
	
	while ((id = poll(ES_COMM_PORT))) {
		receive(id, ES_COMM_PORT, &func, sizeof(int));
		funcs[func](id);
	}
}
