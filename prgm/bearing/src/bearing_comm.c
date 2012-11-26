#include <frosk.h>
#include "bearing_intrn.h"
#include <debug.h>

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
	debug_line("Hello World!");
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
//	exp.visible = false;

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

bool check_event(event ev) {
	return ev.type==KEY_DOWN;
}

void handle_events_func() {
	event_list * events;
	event_list * mine;
	event_list * itr;
	event_list * mine_itr;
	event_list * prev = NULL;
	expanse exp;
	
	events = get_events(expsys, ES_COMM_PORT);
	
	// Remove events that pertain to me
	for(itr = events; itr != NULL; itr = itr->next != NULL ? itr->next : prev->next) {
		if(check_event(itr->event)) {
			if(mine == NULL) {
				mine = itr;
				mine_itr = mine;
			} else {
				mine_itr->next = itr;
				mine_itr = mine_itr->next;
			}
			
			if(prev == NULL) {
				events = itr;
			} else {
				prev->next = itr->next;
			}
			itr->next = NULL;
		} else {
			prev = itr;
		}
	}
	
	send_events(expsys, ES_COMM_PORT, events);
	
	for(itr = mine; itr != NULL; itr = itr->next){
		if(itr->event.u.keyboard.letter == 's') {
			send_func(ES_GET_FRONT_EXPANSE);
			receive_data(&exp, sizeof(expanse));
			exp.visible = !exp.visible;
			update_expanse(exp);
		}
	}
}

void serve(void) {
	int func;
	pid_t id;
	while ((id = poll(ES_COMM_PORT))) {
		receive_data(&func, sizeof(int));
		funcs[func]();
	}
}
