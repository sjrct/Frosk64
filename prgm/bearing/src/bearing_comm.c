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
	switch(ev.u.keyboard.letter) {
		case 's':
		case 'j':
		case 'i':
		case 'k':
		case 'l':
			return true;
	}
	return false;
}

void handle_events_func() {
	event_list * events;
	event_list * itr;
	event_list * mine = NULL;
	event_list * mine_itr;
	event_list * new = NULL;
	event_list * new_itr;
	expanse exp;
	events = get_events(expsys, EVENT_COMM_PORT);
	// Remove events that pertain to me
	for(itr = events; itr != NULL; itr = itr->next) {
		if(check_event(itr->event)) {
			if(mine == NULL) {
				mine = malloc(sizeof(event_list));
				mine_itr = mine;
			} else {
				mine_itr->next = malloc(sizeof(event_list));
				mine_itr = mine_itr->next;
			}
			mine_itr->event = itr->event;
		} else {
			if(new == NULL) {
				new = malloc(sizeof(event_list));
				new_itr = new;
			} else {
				new_itr->next = malloc(sizeof(event_list));
				new_itr = new_itr->next;
			}
			new_itr->event = itr->event;
		}
	}
	
	send_events(expsys, EVENT_COMM_PORT, new);
	
	send_func(ES_GET_FRONT_EXPANSE);
	receive_data(&exp, sizeof(expanse));
	for(itr = mine; itr != NULL; itr = itr->next){
		switch(itr->event.u.keyboard.letter) {
			case 's':
				exp.visible = !exp.visible;
				break;
			case 'j':
				exp.x-=3;
				break;
			case 'l':
				exp.x+=3;
				break;
			case 'k':
				exp.y+=3;
				break;
			case 'i':
				exp.y-=3;
				break;
		}
	}
	update_expanse(exp);
	send_func(ES_REENABLE_EVENTS);
	free_event_list(new);
	free_event_list(events);
	free_event_list(mine);
}

void serve(void) {
	int func;
	pid_t id;
	while ((id = poll(ES_COMM_PORT))) {
		receive_data(&func, sizeof(int));
		funcs[func]();
	}
}
