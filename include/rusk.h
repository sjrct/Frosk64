//
// rusk.h
//
// written by naitsirhc
//

#ifndef _RUSK_H_
#define _RUSK_H_

#include <frosk.h>

#define RUSK_COMM_PORT 0x10

typedef long window_handle;

typedef struct api_window {
	int width, height;
	unsigned attributes;
} api_window;

typedef struct window {
	int x, y;
	int sub_offset_x, sub_offset_y;
	int width, height;
	
	api_window api_win;
} window;

enum {
	RUSK_CREATE_WINDOW = 0,
	RUSK_REMOVE_WINDOW,
/*	RUSK_UPDATE_DECORATION_BUFFER,
	RUSK_UPDATE_WINDOW,
	RUSK_UPDATE_PARTIAL_WINDOW*/
};

/*
extern pid_t wm;
extern int wm_port;
*/

#endif
