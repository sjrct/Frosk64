#ifndef BEARING_INTRN_H
#define BEARING_INTRN_H

#include <stdlib.h>
#include <expanse.h>
#include <stdbool.h>
#include <events.h>

bool register_em();
void serve(void);
void update_expanse(expanse);
void resize_expanse(expanse e, int new_width, int new_height);
#endif
