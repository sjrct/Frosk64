//
// ehci.c
//
// written by sjrct
//

#include "ehci.h"
#include "pci.h"

static int ehci_setup(ehci_pci_inf *);

int (*ehci_functions[EHCI_FUNC_COUNT])() = {
	NULL,
	ehci_setup
};

static int ehci_setup(ehci_pci_inf * d)
{
	// TODO
	
	return 0;
}
