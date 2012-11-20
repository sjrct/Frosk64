#include <frosk.h>
#include "bearing_intrn.h"

static pid_t expsys;

// void init_expanse(expanse)
void init_expanse_func();
// void resize_expanse(expanse, handle, width, height)
void resize_func();
// event_list handle_events(event_list)
void handle_events_func();

static void (*funcs[])() = {
	init_expanse_func,
	resize_func,
	handle_events_func,
};

void send_func(int func) {
	send(expsys, ES_COMM_PORT, &func, sizeof(int));
}
void send_data(void * data, int size) {
	send(expsys, ES_COMM_PORT, data, size);
}
void receive_data(void * data, int size) {
	while(!receive(expsys, ES_COMM_PORT, data, size));
}

bool register_em() {
	bool res;
	
	while(!(expsys = get_esys()));
	
	send_func(ES_REGISTER_EM);
	receive_data(&res, sizeof(bool));
	return res;
}

void update_expanse(expanse e) {
	send_func(ES_UPDATE_EXPANSE);
	send_data(&e, sizeof(expanse));
}

void init_expanse_func() {
	expanse exp;
	receive_data(&exp, sizeof(expanse));
	
//	exp.border = 4 ...

	update_expanse(exp);
}

void resize_func() {
	expanse exp;
	expanse_handle handle;
	int width, height;
	
	receive_data(&exp, sizeof(expanse));
	receive_data(&handle, sizeof(expanse_handle));
	receive_data(&width, sizeof(int));
	receive_data(&height, sizeof(int));
	
	resize_expanse(exp, width, height);
}

void handle_events_func() {
	event_list * events;
	
	events = get_events(expsys, ES_COMM_PORT);
	
	// Remove events that pertain to me
	
	send_events(expsys, ES_COMM_PORT, events);
}

void serve(void) {
	int func;
	pid_t id;
	while ((id = poll(ES_COMM_PORT))) {
		receive_data(&func, sizeof(int));
		funcs[func]();
	}
}
