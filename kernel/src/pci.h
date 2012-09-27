//
// pci.h
//
// written by sjrct
//

#ifndef PCI_H
#define PCI_H

#include "cdef.h"

#define MAX_BUSES 0x100
#define MAX_DEVS  0x20

#define PCI_DEV_BM_ELM(B,D) (B * (MAX_DEVS / 8) + D / 8)

uint pci_read(uchar, uchar, uchar, uchar);
void pci_write(uchar, uchar, uchar, uchar, uint);
void pci_detection(uchar[], uint);

#endif
