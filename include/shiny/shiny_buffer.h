//
// shiny_buffer.h
//
// written by naitsirhc
//

#ifndef _SHINY_BUFFER_H_
#define _SHINY_BUFFER_H_

#include <shiny/shiny_thingy_prototype.h>
#include <bufferutils.h>

typedef struct shiny_buffer shiny_buffer;

shiny_thingy * create_shiny_buffer();
void draw_shiny_buffer(shiny_buffer *, shiny_loc, shiny_size);
void set_shiny_buffer(shiny_buffer * thingy, pixel_buffer buff);

#endif
