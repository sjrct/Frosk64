//
// shiny.h
//
// written by naitsirhc
//

#ifndef _SHINY_H_
#define _SHINY_H_

#include <expanse.h>
#include <stdbool.h>
#include <events.h>
#include <shiny/shiny_thingy.h>

shiny_thingy * create_shiny_expanse(int width, int height);
void destroy_expanse(expanse_handle handle);
void draw_buffer(pixel_buffer buffer, shiny_loc loc);
void register_event_handler(shiny_thingy * thingy, event_type type, bool (*handler_func)(shiny_thingy *, event));
void shiny_main_loop();

#endif
