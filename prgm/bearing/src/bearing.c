#include "bearing_intrn.h"
#include <debug.h>

int main() {
	if(!register_em()) {
		return 1;
	}	

	while(1) {
		serve();
	}
	return 0;
}

static bool mouse_down;

bool in_range_header(event ev, expanse exp) {
	return  ev.u.mouse.x >= exp.x &&
			ev.u.mouse.x <= exp.x + exp.width &&
			ev.u.mouse.y >= exp.y + exp.sub_offset_y && 
			ev.u.mouse.y <= exp.y;
}
bool in_range(event ev, expanse exp) {
	return  ev.u.mouse.x >= exp.x && ev.u.mouse.x <= exp.x + exp.width &&
			ev.u.mouse.y >= exp.y && ev.u.mouse.y <= exp.y + exp.height;
}
bool in_resize(event ev, expanse exp) {
	bool a = ev.u.mouse.x >= exp.x + exp.width - 10 && ev.u.mouse.x <= exp.x + exp.width;
	bool b = ev.u.mouse.y >= exp.y + exp.height - 10 && ev.u.mouse.y <= exp.y + exp.height;
	return a && b;
}

bool check_event(event ev, expanse_list * expanses) {
	expanse_list * itr;
	switch(ev.type) {
		case KEY_DOWN:
			switch(ev.u.keyboard.letter) {
				case 's':
					return true;
			}
		break;
		case MOUSE_DOWN:
			// in front expanse (area already selected)
			if(in_range(ev, expanses->exp) && !in_resize(ev, expanses->exp)) {
				return false;
			}
			for(itr = expanses; itr != NULL; itr = itr->next) {
				// in or on the header of the expanse
				if (in_range_header(ev,itr->exp) || in_range(ev, itr->exp)) {
					mouse_down = true;
					return true;
				}
			}
		break;
		case MOUSE_UP:
			if(mouse_down) {
				mouse_down = false;
				return true;
			}
		break;
		default:
			return false;
	}
	return false;
}

event_list * handle_events(event_list * events, expanse_list * expanses) {
static	int mdx, mdy;
static	expanse exp;
static bool resize = false;

	pixel f = { 0, 0, 100};
	pixel b = { 40, 40, 40};
	event_list * itr;
	event_list * mine = NULL;
	event_list * mine_itr;
	event_list * new = NULL;
	event_list * new_itr;
	expanse_list * exp_itr;

	// Remove events that pertain to me
	for(itr = events; itr != NULL; itr = itr->next) {
		if(check_event(itr->event, expanses)) {
			if(mine == NULL) {
				mine = malloc(sizeof(event_list));
				mine_itr = mine;
			} else {
				mine_itr->next = malloc(sizeof(event_list));
				mine_itr = mine_itr->next;
			}
			mine_itr->event = itr->event;
		} else {
			if(new == NULL) {
				new = malloc(sizeof(event_list));
				new_itr = new;
			} else {
				new_itr->next = malloc(sizeof(event_list));
				new_itr = new_itr->next;
			}
			new_itr->event = itr->event;
		}
	}
	
	for(itr = mine; itr != NULL; itr = itr->next){
		switch(itr->event.type) {
			case KEY_DOWN:
				switch(itr->event.u.keyboard.letter) {
					case 's':
						exp.visible = !exp.visible;
						update_expanse(exp);
						break;
				}
				break;
			case MOUSE_DOWN:
				mdx = itr->event.u.mouse.x;
				mdy = itr->event.u.mouse.y;
				for(exp_itr = expanses; exp_itr != NULL; exp_itr = exp_itr->next) {
					if(in_resize(itr->event, exp_itr->exp)) {
						if (exp.handle != (exp_itr->exp.handle)) {
							setup_borders(exp, b);
							exp = exp_itr->exp;
							set_front_expanse(exp);
							setup_borders(exp, f);
						}
						resize = true;
						break;
					}
					if(in_range(itr->event, exp_itr->exp) || in_range_header(itr->event, exp_itr->exp)){
						resize = false;
						setup_borders(exp, b);
						exp = exp_itr->exp;
						set_front_expanse(exp);
						setup_borders(exp, f);
						break;
					}
				}
				break;
			case MOUSE_UP:
				if(resize) {
					exp.width += itr->event.u.mouse.x - mdx;
					exp.height += itr->event.u.mouse.y - mdy;
					update_expanse(exp);
					setup_borders(exp, f);
				} else {
					exp.x += itr->event.u.mouse.x - mdx;
					exp.y += itr->event.u.mouse.y - mdy;
					update_expanse(exp);
				}
				break;
			default:
				break;
		}
	}
	free_event_list(mine);
	return new;
}

// from handle event or expanse system
void resize_expanse(expanse e, int new_width, int new_height) {
	e.api_exp.width = new_width;
	e.api_exp.height = new_height;
//	set_expanse_geometry(&e);
	update_expanse(e);
//	update_decoration_buffer(e);
}

void setup_borders(expanse exp, pixel p) {
	pixel_buffer top;
	top = create_buffer(exp.width, -exp.sub_offset_y, p);
	update_borders(exp, top);
}

