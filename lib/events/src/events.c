//
// events.c
//
// written by naitsirhc
//

#include <frosk.h>
#include <expanse.h>
#include <stdbool.h>
#include <stdlib.h>
#include <events.h>

event_list * get_events(pid_t pid, int port) {
	event_list * events = NULL;
	event_list * itr;
	event e;
	bool has_event = true;
	
	while (has_event) {
		has_event = receive(pid, port, &e, sizeof(e));
		if (has_event) {
			if (events == NULL) {
				events = malloc(sizeof(event_list));
				events->event = e;
				itr = events;
			}
			else {
				itr->next = malloc(sizeof(event_list));
				itr = itr->next;
				itr->event = e;
			}
		}
	}
	
	return events;
}

void send_events(pid_t pid, int port, event_list * list) {
	for(; list->next != NULL; list = list->next) {
		send(pid, port, &list->event, sizeof(event));
	}
}
