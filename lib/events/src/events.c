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

event_list * get_events() {
	event_list * events = NULL;
	event_list * itr;
	event e;
	bool has_event = true;
	pid_t expsys_pid = get_esys();
	
	while (has_event) {
		has_event = receive(expsys_pid, ES_COMM_PORT, &e, sizeof(e));
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
