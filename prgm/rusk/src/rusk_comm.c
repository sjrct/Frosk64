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
// void set_front_expanse(expanse)
static void set_front_expanse_func(pid_t);
// void reenable_events()
static void send_events_func(pid_t);
// void update_decoration_buffer(expanse, pixel_buffer top)
static void update_decoration_buffer_func(pid_t);

static void (*funcs[])(pid_t) = {
	create_expanse_func,
	remove_expanse_func,
	update_partial_expanse_func,
	update_expanse_func,
	register_em_func,
	set_front_expanse_func,
	send_events_func,
	update_decoration_buffer_func,
};

void receive_data(pid_t pid, int port, void * data, int size) {
	receive(pid, port, data, size);
}

void call_func(pid_t id, int func) {
	send(id, ES_COMM_PORT, &func, sizeof(int));
}

static void create_expanse_func(pid_t id) {
	expanse e;
	api_expanse exp;

	receive_data(id, ES_COMM_PORT, &exp, sizeof(api_expanse));
	e = add_expanse(&exp, id);
	
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

	receive_data(id, ES_COMM_PORT, &handle, sizeof(handle));
	remove_expanse(*handle);
}

static void update_partial_expanse_func(pid_t id) {
	pixel_buffer p;
	int x, y;
	expanse_handle handle;
	receive_data(id, ES_COMM_PORT, &handle, sizeof(expanse_handle));
	receive_data(id, ES_COMM_PORT, &x, sizeof(int));
	receive_data(id, ES_COMM_PORT, &y, sizeof(int));
	p = receive_buffer(id, ES_COMM_PORT);

	update_partial(handle, p, x, y);
}

static void update_expanse_func(pid_t id) {
	expanse e;
	receive_data(id, ES_COMM_PORT, &e, sizeof(expanse));
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

static void set_front_expanse_func(pid_t id){
	expanse exp;
	receive_data(id, ES_COMM_PORT, &exp, sizeof(expanse));
	bring_expanse_to_front(exp.handle);
}

void handle_events(event_list* events) {
	full_expanse * exp;
	full_expanse * itra	;
	expanse_list * exp_list = NULL;
	expanse_list * itrb;
	
	exp = get_front_expanse();
	if(em) {
		for(itra = exp; itra != NULL; itra = itra->next) {
			if(exp_list == NULL) {
				exp_list = malloc(sizeof(expanse_list));
				itrb = exp_list;
			} else {
				itrb->next = malloc(sizeof(expanse_list));
				itrb = itrb->next;
			}
			itrb->exp = itra->exp;
		}
		
		call_func(em, EM_HANDLE_EVENTS);
		send_exp_list(em, EVENT_COMM_PORT, exp_list);
		send_events(em, EVENT_COMM_PORT, events);
		free_event_list(events);
	}
}

static void send_events_func(pid_t pid) {
	event_list* events = get_events(em, EVENT_COMM_PORT);
	full_expanse * exp = get_front_expanse();
	
	if(exp != NULL) {
		adjust_events(events);
		send(exp->pid, EVENT_COMM_PORT, &exp->exp.handle, sizeof(expanse_handle));
		send_events(exp->pid, EVENT_COMM_PORT, events);
	}
}

static void update_decoration_buffer_func(pid_t id) {
	pixel_buffer top;
	expanse exp;
	receive_data(id, ES_COMM_PORT, &exp, sizeof(expanse));
	top = receive_buffer(id, ES_COMM_PORT);
	set_decoration_buffers(exp, top);
}

void serve(void) {
	pid_t id;
	int func;
	while ((id = poll(ES_COMM_PORT))) {
		receive(id, ES_COMM_PORT, &func, sizeof(int));
		funcs[func](id);
	}
}
