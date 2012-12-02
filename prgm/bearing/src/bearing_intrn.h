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
void resize_expanse(expanse e, int new_width, int new_height);
void setup_borders(expanse);
void update_borders(expanse exp, pixel_buffer top);
#endif
