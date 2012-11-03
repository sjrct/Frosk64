//
// events.h
//
// written by naitsirhc
//

#ifndef _EVENTS_H_
#define _EVENTS_H_

typedef enum event_type{
	MOUSE_OVER = 0,
	CLICK,
	DBL_CLICK,
	MOUSE_DOWN,
	MOUSE_UP,
	KEY_DOWN,
	KEY_UP,
} event_type;

typedef struct event {
	event_type type;
	int x, y;
	
	union {
		struct {
			char letter;
			int modifiers; //TODO ENUM
		} keyboard;
	} u;
} event;

typedef struct event_list {
	event e;
	struct event_list * next;
} event_list;

event_list * get_events();

#endif
