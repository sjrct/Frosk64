#
# Makefile for the Frosk kernel
#
# written by sjrct
#

SHELL   = /bin/sh

TARGET  = kernel.bin

CSRC_1  = kmain.c interrupt.c exceptions.c kernobj.c pagemgr.c paging.c \
          cga_text.c process.c ata.c keyboard.c fs.c kstrlib.c pci.c driver.c \
          ehci.c fbe.c vesa.c graphics.c ports.c wsysreg.c mouse.c \
	  serial.c
ASRC_1  = exceptions.asm irq.asm paging.asm scheduler.asm keyboard.asm \
          syscall.asm lock.asm tss.asm mouse.asm

CC      = gcc

ASFLAGS = -felf64 -I$(TOPDIR)/kernel/src/
CCFLAGS = -c -Wall -m64 -ffreestanding -nostdinc -mcmodel=large -mno-red-zone -I$(TOPDIR)/include
LDFLAGS = -m64 -ffreestanding -nostdlib -mcmodel=large -mno-red-zone -Tkernel.ld

CSRC    = $(addprefix src/,$(CSRC_1))
CTMP    = $(CSRC:.c=.o)
COBJ    = $(CTMP:src/%=cobj/%)

ASRC    = $(addprefix src/,$(ASRC_1))
ATMP    = $(ASRC:.asm=.o)
AOBJ    = $(ATMP:src/%=aobj/%)

all: $(TARGET)

$(TARGET): cobj aobj $(COBJ) $(AOBJ)
	$(cmd) "$(CC) -o $(TARGET) $(COBJ) $(AOBJ) $(LDFLAGS)" "CC $(TARGET)"

cobj:
	$(cmd) "mkdir cobj" "MKDIR cobj"

aobj:
	$(cmd) "mkdir aobj" "MKDIR aobj"

cobj/%.o: src/%.c
	$(cmd) "$(CC) $(CCFLAGS) -o $@ $<" "CC $@"

aobj/%.o: src/%.asm
	$(cmd) "nasm $(ASFLAGS) -o $@ $<" "NASM $@"

.PHONY: clean
clean:
	-$(cmd) "$(RM) $(TARGET)" "RM $(TARGET)"
	-$(cmd) "$(RM) -rf cobj aobj" "RM cobj aobj"
