//
// shiny.c
//
// written by naitsirhc
//

#include <expanse.h>
#include <stdbool.h>
#include <events.h>
#include <frosk.h>
#include <stdlib.h>
#include <shiny/shiny.h>


typedef struct expanse_list {
	expanse_handle handle;
	shiny_thingy * expanse;
	struct expanse_list * next;
} expanse_list;

typedef struct event_handler {
	shiny_thingy * thingy;
	event_type type;
	bool (*handler_func)(shiny_thingy *, event);
} event_handler;

typedef struct event_handler_list {
	event_handler handler;
	struct event_handler_list * next;
} event_handler_list;

event_handler_list * eh_list;
expanse_list * exp_list;


void call_func(int func) {

}

void send_data(void * data, int size) {
	
}

expanse_handle recieve_handle() {
	expanse_handle h;
	while(!receive(0, ES_COMM_PORT, &h, sizeof(expanse_handle)));
	return h;
}


shiny_thingy * create_shiny_expanse(int width, int height) {
	api_expanse exp;
	expanse_handle handle;
	shiny_thingy * expanse;
	expanse_list * itr;
	
	
	exp.width = width;
	exp.height = height;
	
	
	call_func(ES_CREATE_EXPANSE);
	send_data(&exp, sizeof(exp));
	
	handle = recieve_handle();
	
	expanse = create_container(width, height);
	
	if(exp_list == NULL) {
		exp_list = malloc(sizeof(expanse_list));
		itr = exp_list;
	} else {
		for(itr = exp_list; itr->next != NULL; itr = itr->next);
		itr->next = malloc(sizeof(expanse_list));
		itr = itr->next;
	}
	
	itr->expanse = expanse;
	itr->handle = handle;
	
	return expanse;
}

void remove_expanse(expanse_handle handle) {
	expanse_list * itr;
	expanse_list * tmp;
	
	if(exp_list == NULL){
		return; //TODO error?
	}
	if(exp_list->handle == handle) {
		tmp = exp_list;
		if(exp_list->next != NULL) {
			exp_list = exp_list->next;
		}
		free(tmp);
		return;
	}
	
	for(itr = exp_list; itr->next != NULL; itr = itr->next) {
		if(itr->next->handle == handle) {
			tmp = itr->next;
			itr->next = tmp->next;
			free(tmp);
			return;
		}
	}
	
	// TODO free expanse and it's thingys
}

void destroy_expanse(expanse_handle handle) {
	call_func(ES_REMOVE_EXPANSE);
	send_data(&handle, sizeof(handle));
	
	remove_expanse(handle);
}
/*
void draw_buffer(expanse_handle handle, pixel_buffer buffer, shiny_loc loc) {
	call_func(UPDATE_PARTIAL_EXPANSE);
	send_int(expanse_handle);
	send_int(loc.x);
	send_int(loc.y);
	send_int(buffer.width);
	send_int(buffer.height);
	send_buffer(buffer);
}*/

void register_event_handler(shiny_thingy * thingy, event_type type, bool (*handler_func)(shiny_thingy *, event)) {
	event_handler_list * itr;
	event_handler handler;
	
	handler.handler_func = handler_func;
	handler.type = type;
	handler.thingy = thingy;
	
	if(eh_list == NULL) {
		eh_list = malloc(sizeof(event_handler_list));
		eh_list->handler = handler;
	} else {
		for(itr = eh_list; itr->next != NULL; itr = itr->next);
		itr->next = malloc(sizeof(event_handler_list));
		itr = itr->next;
		itr->handler = handler;
	}
}

bool in_range(event ev, shiny_thingy * thingy) {
	shiny_loc loc = thingy->loc;
	shiny_size size = thingy->size;
	return 
		loc.x <= ev.x &&
		loc.y <= ev.y &&
		loc.x + size.width  >= ev.x &&
		loc.y + size.height >= ev.y;
}

event_list * events_in_window(event_list * events, expanse_handle handle){
	event_list * new, * itr;
	new = NULL;
	
	for(itr = events; itr != NULL; itr = itr->next) {
		if(itr->event.exp_handle == handle) {
			if(new == NULL) {
				new = malloc(sizeof(event_list));
			} else {
				new->next = malloc(sizeof(event_list));
				new = new->next;
			}
			new->event = itr->event;
		}
	}
	return new;
}

void shiny_main_loop() {
	event_handler_list * eh_itr;
	
	event_handler handler;
	event event;
	shiny_thingy * thingy;
	
	event_list * el_itr;
	expanse_list * itr;
	event_list * el;
	event_list * all_el = get_events();
	
	
	// Might be able to do better
	for(itr = exp_list; itr != NULL; itr = itr->next) {
		el = events_in_window(all_el, itr->handle);
	
		for(el_itr = el; el_itr != NULL; el_itr = el_itr->next) {
			for(eh_itr = eh_list; eh_itr != NULL; eh_itr = eh_itr->next) {
				handler = eh_itr->handler;
				thingy = handler.thingy;
				event = el_itr->event;
				
				if (thingy->exp_handle == itr->handle 	&&	// Same expanse
					event.type == handler.type			&& 	// Same type
					in_range(event, thingy)				&&	// Same location
					handler.handler_func(thingy, event)) {	// Event was handled
						break;
				}
			}
		}
	}
}