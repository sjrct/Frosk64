//
// shiny_container.h
//
// written by naitsirhc
//

#ifndef _SHINY_CONTAINER_H_
#define _SHINY_CONTAINER_H_

#include <shiny/shiny_thingy_prototype.h>

typedef struct shiny_container shiny_container;

shiny_thingy * create_shiny_container(int width, int height);

void container_add_thingy(shiny_container *, shiny_thingy *);
void container_remove_thingy(shiny_container *, shiny_thingy *);

void draw_shiny_container(shiny_container *, shiny_loc, shiny_size);

#endif
