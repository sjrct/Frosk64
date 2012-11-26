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
#include <debug.h>

event_list * get_events(pid_t pid, int port) {
	int count;
	event_list * events = NULL;
	event_list * itr;
	event e;
	while(!receive(pid, port, &count, sizeof(int)));
	for(; count != 0; count--) {
		while(!receive(pid, port, &e, sizeof(e)));
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
	
	return events;
}

void send_events(pid_t pid, int port, event_list * list) {
	int count = 0;
	event_list * itr;
	
	for(itr = list; itr != NULL; itr = itr->next) count++;
	send(pid, port, &count, sizeof(int));
	
	for(; list != NULL; list = list->next) {
		send(pid, port, &list->event, sizeof(event));
	}
}
