//
// time.c
//
// written by sjrct
//

#include <time.h>
#include <string.h>
#include <stdbool.h>

inline static bool isleapyear(i)
{
	if (i % 4) return false;
	if (i % 100) return true;
	if (i % 400) return false;
	return true;
}

// unlike the standard, gets clocks elapsed since startup and not since the
//   begining of the program's execution
clock_t clock(void)
{
	unsigned ax, dx;
	clock_t r;

	asm("rdtsc" : "=a" (ax), "=d" (dx));

	r = ((clock_t)dx) << 32;
	r |= ax;
	
	return r;
}

time_t mktime(struct tm * tm)
{
	int i;
	time_t t = 0;
	
	for (i = 1970; i < tm->tm_year; i++) {
		t += 365 * 24 * 60 * 60;
		if (isleapyear(i)) t += 24 * 60 * 60;
	}
	
	t += tm->tm_yday * 24 * 60 * 60;
	t += tm->tm_hour * 60 * 60;
	t += tm->tm_min * 60;
	t += tm->tm_sec;
	
	return t;
}

char * asctime(const struct tm * tm)
{
	static const char * wday_names[] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	
	static const char * mon_names[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

                            //012345678902343567890123
	static char timestr[] = "Www Mmm dd hh:mm:ss yyyy\n";
	
	memcpy(timestr, wday_names[tm->tm_wday], 3);
	memcpy(timestr + 4, mon_names[tm->tm_mon], 3);
	
	// TODO
	
	return timestr;
}
