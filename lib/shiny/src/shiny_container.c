//
// shiny_container.c
//
// written by naitsirhc
//

#include <stdlib.h>
#include <shiny/shiny.h>
#include <shiny/shiny_container.h>

typedef struct container_list {
	shiny_thingy * thingy;
	struct container_list * next;
} container_list;

struct shiny_container{
	container_list * things;
//	color bg_color;
//	int border_width;
};


shiny_thingy * create_container(int width, int height) {
	shiny_thingy * thingy = malloc(sizeof(shiny_thingy));
	SHINY_CONTAINER(thingy) = malloc(sizeof(shiny_container));
	thingy->size.width = width;
	thingy->size.height = height;
	return thingy;
}

void add_thingy(shiny_container * container, shiny_thingy * thingy) {
	container_list * itr;
	
	if(container->things != NULL) {
		for(itr = container->things; itr->next != NULL; itr = itr->next);
		itr->next = malloc(sizeof(container_list));
		itr = itr->next;
	} else {
		container->things = malloc(sizeof(container_list));
		itr = container->things;
	}
	
	itr->thingy = thingy;
}

void remove_thingy(shiny_container * container, shiny_thingy * thingy) {
	container_list * itr = container->things;
	container_list * found;
	
	if(itr != NULL) {
		if(itr->next == NULL) {
			if(itr->thingy == thingy) {
				free(itr);
				container->things = NULL;
			}
			return;
		}
		for(; itr->next != NULL; itr = itr->next) {
			if(itr->next->thingy == thingy) {
				if(itr->next->next == NULL) {
					free(itr->next);
					itr->next = NULL;
				} else {
					found = itr->next;
					itr->next = itr->next->next;
					free(found);
				}
			}
		}
	}
}

void draw_shiny_container(shiny_container * container, shiny_loc loc, shiny_size size) {
	container_list * itr;
	
	// TODO change size
	
	// TODO draw border
	
	//fill_rect(loc->x, loc->y, width, height, bg_color);
	
	//TODO straighten out how the drawing is done and how the location is passed
	
	if (container->things != NULL) {
		for (itr = container->things; itr != NULL; itr = itr->next) {
			itr->thingy->loc = loc;
			//TODO change itr->thingy size
			draw(itr->thingy, loc);
			loc.x += itr->thingy->size.width;
		}
	}
}
