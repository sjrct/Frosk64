#include <frosk.h>
#include "bearing_intrn.h"
#include <debug.h>

static pid_t expsys;

// void init_expanse(expanse)
void init_expanse_func();
// void resize_expanse(expanse, handle, width, height)
void resize_func();
// event_list handle_events(expanse_list, event_list)
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
	while(!poll(ES_COMM_PORT));
	receive(expsys, ES_COMM_PORT, data, size);
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

void update_borders(expanse exp, pixel_buffer top) {
	send_func(ES_UPDATE_DECORATION_BUFFER);
	send_data(&exp, sizeof(expanse));
	send_buffer(expsys, ES_COMM_PORT, top);
}

static bool old = false;
static expanse oldexp;
static int last_x = 30;
void init_expanse_func() {
	expanse exp;
	pixel p = { 40, 40, 40};
	pixel b = { 0, 0, 100};
	receive_data(&exp, sizeof(expanse));
	
	exp.y = 30;
	exp.x = last_x;
	last_x += exp.width + 10;
	exp.sub_offset_x = 0;
	exp.sub_offset_y = -10;
	
	update_expanse(exp);
	
	setup_borders(exp, p);
	
	if(old) {
//		setup_borders(oldexp, p);
	}
	old = true;
	
	oldexp = exp;
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

void set_front_expanse(expanse exp) {	
	send_func(ES_SET_FRONT_EXPANSE);
	send_data(&exp, sizeof(expanse));
}

void handle_events_func() {
	event_list * events;
	expanse_list * expanses;
	
	expanses = receive_exp_list(expsys, EVENT_COMM_PORT);
	events = get_events(expsys, EVENT_COMM_PORT);
	
	events = handle_events(events, expanses);
	
	send_func(ES_SEND_EVENTS);
	send_events(expsys, EVENT_COMM_PORT, events);
	
	free_event_list(events);
}

void serve(void) {
	int func;
	pid_t id;
	while ((id = poll(ES_COMM_PORT))) {
		receive_data(&func, sizeof(int));
		funcs[func]();
	}
}
