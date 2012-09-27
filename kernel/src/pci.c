//
// pci.c
//
// written by sjrct
//

#include "pci.h"
#include "inline_asm.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA    0xCFC

#define BUS_BIT0 16
#define DEV_BIT0 11
#define FUN_BIT0 8
#define REG_BIT0 2

#define PCI_RW_COMMON \
	uint addr = 0x80000000; \
	addr |= (ulong)bus << BUS_BIT0; \
	addr |= (ulong)dev << DEV_BIT0; \
	addr |= (ulong)fun << FUN_BIT0; \
	addr |= (ulong)reg << REG_BIT0; \
	OUTD(CONFIG_ADDRESS, addr);

uint pci_read(uchar bus, uchar dev, uchar fun, uchar reg)
{
	uint val;
	PCI_RW_COMMON
	IND(val, CONFIG_DATA);
	return val;
}

void pci_write(uchar bus, uchar dev, uchar fun, uchar reg, uint val)
{
	PCI_RW_COMMON
	OUTD(CONFIG_DATA, val);
}

void pci_detection(uchar dev_bm[], uint sz)
{
	uint bus, dev, res;
	
	for (bus = 0; bus < MAX_BUSES; bus++) {
		for (dev = 0; dev < MAX_DEVS; dev++) {
			if ((dev & 0xf) == 0) {
				if (PCI_DEV_BM_ELM(dev, bus) >= sz) return;
				dev_bm[PCI_DEV_BM_ELM(dev, bus)] = 0;
			}

			res = pci_read(bus, dev, 0, 0);

			if ((res & 0xffff) != 0xffff) {
				dev_bm[PCI_DEV_BM_ELM(dev, bus)] |= 1 << dev / 8;
			}
		}
	}
}
