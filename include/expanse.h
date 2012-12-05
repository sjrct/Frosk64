//
// expanse.h
//
// written by naitsirhc
//

#ifndef _EXPANSE_H_
#define _EXPANSE_H_

#define ES_COMM_PORT 0x10

#include <stdbool.h>
#include <frosk.h>

typedef long expanse_handle;

// Expanse Manager
enum {
	EM_INIT_EXPANSE = 0,
	EM_RESIZE_EXPANSE,
	EM_HANDLE_EVENTS,
};

// Expanse System
enum {
	ES_CREATE_EXPANSE = 0,
	ES_REMOVE_EXPANSE,
	ES_UPDATE_EXPANSE_API_BUFFER,
	ES_UPDATE_EXPANSE,
	ES_REGISTER_EM,
	ES_GET_FRONT_EXPANSE,
	ES_REENABLE_EVENTS,
	ES_UPDATE_DECORATION_BUFFER,
};

// Expanse API?

typedef struct {
	int width, height;
	unsigned attributes;
} api_expanse;

typedef struct {
	int x, y;
	int sub_offset_x, sub_offset_y;
	int width, height;
	bool visible;
	expanse_handle handle;

	api_expanse api_exp;
} expanse;


typedef struct expanse_list {
	expanse exp;
	struct expanse_list * next;
} expanse_list;

void send_exp_list(pid_t id, int port, expanse_list *);
expanse_list * receive_exp_list(pid_t id, int port);
void free_exp_list(expanse_list *);

#endif
