//
// rusk_intrn.h
//
// written by naitsirhc
//

#ifndef RUSK_INTRN_H
#define RUSK_INTRN_H

#include <frosk.h>

void serve(void);

void register_wm(pid_t window_manager);
window_handle add_window(const api_window*);
void remove_window(window_handle w);

#endif
