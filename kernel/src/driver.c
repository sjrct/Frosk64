//
// driver.c
//
// written by sjrct
//

#include "driver.h"
#include "ehci.h"
#include "keyboard.h"
#include "cga_text.h"

#define ACTIVE   1
#define BUILTIN  2
#define TYPE_BIT 2

typedef struct {
	uchar flags;
	uchar priority;
	unsigned function_count;
	int (**functions)();
	char * filename;
} driver;

#define BUILTIN_DRIVER(I,F,T) \
	drivers[I].flags = ACTIVE | BUILTIN | ((T) << TYPE_BIT); \
	drivers[I].priority = 0; \
	drivers[I].function_count = sizeof(F ## _functions); \
	drivers[I].functions = F ## _functions; \
	drivers[I].filename = NULL; \
	driver_call(I, DRIVER_FUNC_INIT, NULL)

static driver drivers[MAX_DRIVERS];

void init_drivers(void)
{
	BUILTIN_DRIVER(DRIVER_ID_EHCI, ehci, DRIVER_OTHER);
	BUILTIN_DRIVER(DRIVER_ID_KEYBOARD, keyboard, DRIVER_SCHAR);
	BUILTIN_DRIVER(DRIVER_ID_CGA_TEXT, cga_text, DRIVER_OTHER);

	// TODO read additional drivers from config file
}

int driver_call(uint drvr, uint func, void * params)
{
	if (drvr >= MAX_DRIVERS) return -1;
	if (~drivers[drvr].flags & ACTIVE) return drivers[drvr].flags;
	if (func == -1) return 0;
	if (func >= drivers[drvr].function_count) return -3;
	if (drivers[drvr].functions[func] == NULL) return 0;

	return drivers[drvr].functions[func](params);
}
