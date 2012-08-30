//
// time.h
//
// written by sjrct
//

#ifndef _TIME_H_
#define _TIME_H_

#include <stddef.h>

typedef unsigned long long clock_t;
typedef unsigned long long time_t;

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_hour;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

clock_t clock();
time_t time(time_t *);

double difftime(time_t, time_t);

#endif
