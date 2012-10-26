//
// wsysreg.c
//
// written by sjrct
//

#include "wsysreg.h"
#include "kernobj.h"

extern kern_obj * current_thread;

static kern_obj * wsys;

ulong get_wsys(void)
{
	return (ulong)wsys;
}

int reg_wsys(void)
{
	if (wsys == NULL && current_thread != NULL) {
		wsys = current_thread->u.thrd.proc;
		return 1;
	} else {
		return 0;
	}
}

int unreg_wsys(void)
{
	if (wsys == NULL) return 1;
	
	if (current_thread != NULL && wsys == current_thread->u.thrd.proc) {
		wsys = NULL;
		return 1;
	}
	
	return 0;
}
