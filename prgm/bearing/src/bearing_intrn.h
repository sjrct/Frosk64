#ifndef BEARING_INTRN_H
#define BEARING_INTRN_H

#include <stdlib.h>
#include <expanse.h>
#include <stdbool.h>
#include <events.h>
#include <bufferutils.h>

bool register_em();
void serve(void);
void update_expanse(expanse);
void setup_borders(expanse, pixel);
void update_borders(expanse exp, pixel_buffer top);
event_list * handle_events(event_list * events, expanse_list * expanses);
void set_front_expanse(expanse exp);
#endif
