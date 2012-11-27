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
// expanse get_front_expanse()
static void get_front_expanse_func(pid_t);

static void (*funcs[])(pid_t) = {
	create_expanse_func,
	remove_expanse_func,
	update_partial_expanse_func,
	update_expanse_func,
	register_em_func,
	get_front_expanse_func,
};

void call_func(pid_t id, int func) {
	send(id, ES_COMM_PORT, &func, sizeof(int));
}

static void create_expanse_func(pid_t id) {
	expanse e;
	api_expanse exp;

	while (!receive(id, ES_COMM_PORT, &exp, sizeof(api_expanse)));
	e = add_expanse(&exp, id);
	
	debug_number(em);
	if(em) {
		em_init_expanse(e);
	}
	// send expanse_handle to process id
	send(id, ES_COMM_PORT, &e.handle, sizeof(expanse_handle));
}

void em_init_expanse(expanse e) {
	call_func(em, EM_INIT_EXPANSE);
	send(em, ES_COMM_PORT, &e, sizeof(expanse));
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
	bool result = em == 0;
	send(id, ES_COMM_PORT, &result, sizeof(bool));

	if(result) {
		em = id;
		em_registered();
	}
}

static void get_front_expanse_func(pid_t id) {
	full_expanse * fexp;
	expanse exp;
	
	fexp = get_front_expanse();
	if(fexp != NULL) {
		exp = fexp->exp;
	} else {
		exp.handle = -1;
	}
	send(id, ES_COMM_PORT, &exp, sizeof(expanse));
}

void handle_events(event_list* events) {
	full_expanse * exp;
	if(em) {
		call_func(em, EM_HANDLE_EVENTS);
		send_events(em, EVENT_COMM_PORT, events);
		free(events);
		events = get_events(em, EVENT_COMM_PORT);
	}
	exp = get_front_expanse();
	if(exp != NULL) {
		// the following events have been modified from their original version to fit your expanse
		adjust_events(events);
		send(exp->pid, EVENT_COMM_PORT, exp->exp.handle, sizeof(expanse_handle));
		send_events(exp->pid, EVENT_COMM_PORT, events);
	}
}

void serve(void) {
	pid_t id;
	int func;
	while ((id = poll(ES_COMM_PORT))) {
		receive(id, ES_COMM_PORT, &func, sizeof(int));
		funcs[func](id);
	}
}
