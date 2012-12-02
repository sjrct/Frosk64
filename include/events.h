//
// events.h
//
// written by naitsirhc
//

#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <frosk.h>

#define EVENT_COMM_PORT 0x11

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
	expanse_handle exp_handle;
	event_type type;
	
	union {
		struct {
			char letter;
			bool shift;
			bool ctrl;
			bool alt;
		} keyboard;
		struct {
			int x, y;
			int button;
		} mouse;
	} u;
} event;

typedef struct event_list {
	event event;
	struct event_list * next;
} event_list;

event_list * get_events(pid_t, int);
void send_events(pid_t pid, int port, event_list * list);
void free_event_list(event_list * list);

#endif
