
#include <stdbool.h>
#include <events.h>

event_list get_events() {
	event_list events = NULL;
	event_list itr;
	event e;
	bool has_event = true;
	pid winsys_pid = get_winsys_pid();
	
	while (has_event){
		has_event = request(winsys_pid, port, &e, sizeof(e));
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
}
