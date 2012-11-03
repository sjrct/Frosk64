//
// events.c
//
// written by naitsirhc
//

#include <frosk.h>
#include <stdbool.h>
#include <stdlib.h>
#include <events.h>
#include <rusk.h>

event_list * get_events() {
	event_list * events = NULL;
	event_list * itr;
	event e;
	bool has_event = true;
	pid_t winsys_pid = get_wsys();
	
	while (has_event) {
		has_event = receive(winsys_pid, RUSK_COMM_PORT, &e, sizeof(e));
		if (has_event) {
			if (events == NULL) {
				events = malloc(sizeof(event_list));
				events->e = e;
				itr = events;
			}
			else {
				itr->next = malloc(sizeof(event_list));
				itr = itr->next;
				itr->e = e;
			}
		}
	}
	
	return events;
}
