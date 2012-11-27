//
// rusk_intrn.h
//
// written by naitsirhc
//

#ifndef RUSK_INTRN_H
#define RUSK_INTRN_H

#include <frosk.h>
#include <expanse.h>
#include <events.h>

typedef struct full_expanse {
	expanse exp;
	pixel_buffer expanse_buffer;
	pid_t pid;
	char * lbuf;
	bool dirty_lbuf;
	struct full_expanse * next;
} full_expanse;

void serve(void);

void register_wm(pid_t expanse_manager);
expanse add_expanse(const api_expanse*);
void remove_expanse(expanse_handle w);
void update_partial(expanse_handle handle, pixel_buffer p, int x, int y);
void update_expanse(expanse);
full_expanse * get_front_expanse();
void handle_events(event_list*);

void em_registered();
void em_init_expanse(expanse e);

pid_t em;

#endif
