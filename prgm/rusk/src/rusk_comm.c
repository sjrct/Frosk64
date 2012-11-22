//
// rusk_comm.c
//
// written by naitsirhc
//

#include <frosk.h>
#include <stdlib.h>
#include <bufferutils.h>
#include "rusk_intrn.h"
#include <stdbool.h>
#include <debug.h>

// expanse_handle create_expanse(api_expanse)
static void create_expanse_func(pid_t);
// void remove_expanse(expanse_handle)
static void remove_expanse_func(pid_t);
// void update_partial_expanse(expanse_handle, int x, int y, pixel_buffer)
static void update_partial_expanse_func(pid_t);
// void update_expanse(expanse)
static void update_expanse_func(pid_t);
// bool register_em()
static void register_em_func(pid_t);

static void (*funcs[])(pid_t) = {
	create_expanse_func,
	remove_expanse_func,
	update_partial_expanse_func,
	update_expanse_func,
	register_em_func,
};

void call_func(pid_t id, int func) {
	send(id, ES_COMM_PORT, &func, sizeof(int));
}

static void create_expanse_func(pid_t id) {
	expanse e;
	api_expanse exp;

	while (!receive(id, ES_COMM_PORT, &exp, sizeof(api_expanse)));
	e = add_expanse(&exp);
	
	/*if(em) {
		call_func(em, EM_INIT_EXPANSE);
		send(em, ES_COMM_PORT, &e, sizeof(expanse));
	}*/
	// send expanse_handle to process id
	send(id, ES_COMM_PORT, &e.handle, sizeof(expanse_handle));
}

static void remove_expanse_func(pid_t id) {
	expanse_handle * handle;

	while (!receive(id, ES_COMM_PORT, &handle, sizeof(handle)));
	remove_expanse(*handle);
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
}

static void update_expanse_func(pid_t id) {
	expanse e;
	while(!receive(id, ES_COMM_PORT, &e, sizeof(expanse)));
	update_expanse(e);
}

static void register_em_func(pid_t id) {
	bool result;
	if (em == 0) {
		em = id;
		result = true;
	}
	result = false;
	send(id, ES_COMM_PORT, &result, sizeof(bool));
}

void serve(void) {
	pid_t id;
	int func;
	while ((id = poll(ES_COMM_PORT))) {
		receive(id, ES_COMM_PORT, &func, sizeof(int));
		funcs[func](id);
	}
}
