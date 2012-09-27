//
// exceptions.c
//
// written by sjrct
//

#include "exceptions.h"
#include "interrupt.h"
#include "kerndef.h"

void div0_fault();
void gp_fault();
void dbl_fault();
void undef_int();

#define DIV0_INT 0x0
#define DBL_INT  0x8
#define GPF_INT  0xD

void reg_exceptions(void)
{
	int i;
	for (i = 0; i < 0x100; i++) {
		register_int(i, (ulong)undef_int, KERNEL_CS, 0x8E);
	}

	register_int(DIV0_INT, (ulong)div0_fault, KERNEL_CS, 0x8E);
	register_int(GPF_INT,  (ulong)gp_fault ,  KERNEL_CS, 0x8E);
	register_int(DBL_INT,  (ulong)dbl_fault,  KERNEL_CS, 0x8E);
}
