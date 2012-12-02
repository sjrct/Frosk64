#include <frosk.h>
#include "bearing_intrn.h"
#include <debug.h>

static pid_t expsys;

// void init_expanse(expanse)
void init_expanse_func();
// void resize_expanse(expanse, handle, width, height)
void resize_func();
// event_list handle_events(event_list, expanse front)
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

void init_expanse_func() {
	expanse exp;
	receive_data(&exp, sizeof(expanse));
	
	exp.y = 30;
	exp.sub_offset_x = 0;
	exp.sub_offset_y = -10;

	update_expanse(exp);
	
	setup_borders(exp);
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

static bool mouse_down;

bool check_event(event ev, expanse exp) {
	switch(ev.type) {
		case KEY_DOWN:
			switch(ev.u.keyboard.letter) {
				case 's':
				case 'j':
				case 'i':
				case 'k':
				case 'l':
					return true;
			}
		break;
		case MOUSE_DOWN:
			if (ev.u.mouse.x >= exp.x && ev.u.mouse.x <= exp.x + exp.width &&
				ev.u.mouse.y >= exp.y + exp.sub_offset_y && ev.u.mouse.y <= exp.y) { //TODO FIX TO BORDER
				mouse_down = true;
				return true;
			}
		break;
		case MOUSE_UP:
			if(mouse_down) {
				mouse_down = false;
				return true;
			}
		break;
		default:
			return false;
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
	int mdx, mdy;
	expanse exp;
	events = get_events(expsys, EVENT_COMM_PORT);
	receive(expsys, EVENT_COMM_PORT, &exp, sizeof(expanse));
	
	// Remove events that pertain to me
	for(itr = events; itr != NULL; itr = itr->next) {
		if(check_event(itr->event, exp)) {
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
	
	for(itr = mine; itr != NULL; itr = itr->next){
		switch(itr->event.type) {
			case KEY_DOWN:
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
				break;
			case MOUSE_DOWN:
				mdx = itr->event.u.mouse.x;
				mdy = itr->event.u.mouse.y;
				break;
			case MOUSE_UP:
				exp.x += itr->event.u.mouse.x - mdx;
				exp.y += itr->event.u.mouse.y - mdy;
				break;
			default:
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
