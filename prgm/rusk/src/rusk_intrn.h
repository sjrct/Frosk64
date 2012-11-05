//
// rusk_intrn.h
//
// written by naitsirhc
//

#ifndef RUSK_INTRN_H
#define RUSK_INTRN_H

#include <frosk.h>
#include <expanse.h>

void serve(void);

void register_wm(pid_t expanse_manager);
expanse_handle add_expanse(const api_expanse*);
void remove_expanse(expanse_handle w);
void update_partial(expanse_handle handle, pixel_buffer p, int x, int y);

#endif
