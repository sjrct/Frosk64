//
// expanse.h
//
// written by naitsirhc
//

#ifndef _EXPANSE_H_
#define _EXPANSE_H_

#define ES_COMM_PORT 0x10

typedef long expanse_handle;

// Expanse Manager
enum {
	EM_REFORMAT_EXPANSE = 0,
};

// Expanse System
enum {
	ES_CREATE_EXPANSE = 0,
	ES_REMOVE_EXPANSE,
	ES_UPDATE_PARTIAL_EXPANSE,
	ES_GET_EXPANSES,
	ES_REGISTER_EM
};

typedef struct {
	int width, height;
	unsigned attributes;
} api_expanse;

typedef struct {
	int x, y;
	int sub_offset_x, sub_offset_y;
	int width, height;
	
	api_expanse api_exp;
} expanse;

#endif
