//
// shiny.c
//
// written by naitsirhc
//

#include <stdlib.h>
#include <expanse.h>
#include <stdbool.h>
#include <events.h>
#include <frosk.h>
#include <bufferutils.h>
#include <shiny/shiny.h>
#include <debug.h>

typedef struct event_handler {
	shiny_thingy * thingy;
	event_type type;
	bool (*handler_func)(shiny_thingy *, event);
} event_handler;

typedef struct event_handler_list {
	event_handler handler;
	struct event_handler_list * next;
} event_handler_list;

typedef struct expanseh_list {
	expanse_handle handle;
	shiny_thingy * expanse;
	struct expanseh_list * next;
	event_handler_list * handlers;
} expanseh_list;

expanseh_list * exp_list;
shiny_thingy * down_thingy = NULL;
shiny_thingy * selected = NULL;

pid_t get_esyspid() {
	while(!get_esys());
	return get_esys();
}

void call_func(int func) {
	send(get_esyspid(), ES_COMM_PORT, &func, sizeof(int));
}

void send_data(void * data, int size) {
	send(get_esyspid(), ES_COMM_PORT, data, size);
}

expanse_handle recieve_handle() {
	expanse_handle h;
	while(!receive(get_esyspid(), ES_COMM_PORT, &h, sizeof(expanse_handle)));
	return h;
}

shiny_thingy * create_shiny_expanse(int width, int height) {
	api_expanse exp;
	expanse_handle handle;
	shiny_thingy * expanse;
	expanseh_list * itr;
	
	exp.width = width;
	exp.height = height;
	

	call_func(ES_CREATE_EXPANSE);
	send_data(&exp, sizeof(api_expanse));

	handle = recieve_handle();
	expanse = create_shiny_container(handle, width, height);
	
	if(exp_list == NULL) {
		exp_list = malloc(sizeof(expanseh_list));
		itr = exp_list;
	} else {
		for(itr = exp_list; itr->next != NULL; itr = itr->next);
		itr->next = malloc(sizeof(expanseh_list));
		itr = itr->next;
	}
	
	itr->expanse = expanse;
	itr->handle = handle;
	
	return expanse;
}

void remove_expanse(expanse_handle handle) {
	expanseh_list * itr;
	expanseh_list * tmp;
	
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

void draw_buffer(pixel_buffer buffer, shiny_loc loc) {
	call_func(ES_UPDATE_EXPANSE_API_BUFFER);
	send_data(&loc.expanse_handle, sizeof(expanse_handle));
	send_data(&loc.x, sizeof(int));
	send_data(&loc.y, sizeof(int));
	send_buffer(get_esyspid(), ES_COMM_PORT, buffer);
}

void register_event_handler(shiny_thingy * thingy, event_type type, bool (*handler_func)(shiny_thingy *, event)) {
	expanseh_list * eitr;
	event_handler_list * itr;
	event_handler handler;
	
	handler.handler_func = handler_func;
	handler.type = type;
	handler.thingy = thingy;
	
	for(eitr = exp_list; eitr != NULL; eitr = eitr->next) {
		if(eitr->handle == thingy->loc.expanse_handle) {
			break;
		}
	}
	
	if(eitr != NULL) {
		if(eitr->handlers == NULL) {
			eitr->handlers = malloc(sizeof(event_handler_list));
			eitr->handlers->handler = handler;
		} else {
			for(itr = eitr->handlers; itr->next != NULL; itr = itr->next);
			itr->next = malloc(sizeof(event_handler_list));
			itr = itr->next;
			itr->handler = handler;
		}
	}
}

bool in_range(event ev, shiny_thingy * thingy) {
	shiny_loc loc = thingy->loc;
	shiny_size size = thingy->size;
	switch(ev.type) {
		case KEY_DOWN:
		case KEY_UP:
			return thingy == selected;
		break;
		
		case MOUSE_DOWN:
			if (loc.x <= ev.u.mouse.x &&
				loc.y <= ev.u.mouse.y &&
				loc.x + size.width  >= ev.u.mouse.x &&
				loc.y + size.height >= ev.u.mouse.y) {
				down_thingy = thingy;
				return true;
			}
		break;
		
		case MOUSE_UP:
			if(down_thingy == thingy) {
				//Still in range
				if (loc.x <= ev.u.mouse.x && 
					loc.y <= ev.u.mouse.y &&
					loc.x + size.width  >= ev.u.mouse.x &&
					loc.y + size.height >= ev.u.mouse.y) {
					selected = thingy;
					return true;
				}
			}
			down_thingy = NULL;
		break;
		
		default :
			return false;
	}
	return false;
}

void shiny_main_loop() {
	expanse_handle handle;
	event_handler_list * eh_itr;
	
	event_handler handler;
	event event;
	shiny_thingy * thingy;
	
	event_list * el_itr;
	expanseh_list * itr;
	event_list * el;
	
	shiny_loc loc;
	loc.x = 0;
	loc.y = 0;
	
	for(itr = exp_list; itr != NULL; itr = itr->next) {
		loc.expanse_handle = itr->handle;
		itr->expanse->loc = loc;
		draw(itr->expanse);
	}

	while(1) {
		while(!receive(get_esyspid(), EVENT_COMM_PORT, &handle, sizeof(expanse_handle)));
		el = get_events(get_esyspid(), EVENT_COMM_PORT);
		
		for(itr = exp_list; itr != NULL; itr = itr->next) {
			if(itr->handle == handle) {
				break;
			}
		}
		if(itr == NULL) continue;
		
		for(el_itr = el; el_itr != NULL; el_itr = el_itr->next) {
			for(eh_itr = itr->handlers; eh_itr != NULL; eh_itr = eh_itr->next) {
				handler = eh_itr->handler;
				thingy = handler.thingy;
				event = el_itr->event;
				
				if (in_range(event, thingy)						&&	// Same location
					event.type == handler.type					&& 	// Same type
					handler.handler_func(thingy, event)) {			// Event was handled
						break;
				}
			}
		}
	}
}
