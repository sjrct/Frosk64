#
# makefile for Frosk 64
#
# written by sjrct
#

SHELL      = /bin/sh

TARGET     = frosk.img

UTILS_B    = f301_builder
PRGMS_B    = start

MAKE      := $(MAKE) --no-print-directory
TOPDIR     = $(shell pwd)
export TOPDIR

# handle verbosity
ifndef V
	V = 1
endif
cmdl := $(TOPDIR)/util/verbose/ver$V.sh
cmd  := @$(cmdl)
export cmdl
export cmd

# process simpler variables defined above
UTILDIRS   = $(UTILS_B:%=util/%)
UTILS_PREF = $(addprefix /,$(UTILS_B))
UTILS      = $(join $(UTILDIRS), $(UTILS_PREF))

PRGMDIRS   = $(PRGMS_B:%=prgm/%)
PRGMS_PREF = $(addprefix /,$(UTILS_B))
PRGMS      = $(join $(PRGMDIRS), $(PRGMS_PREF))

# dependecies and commands
all: $(TARGET)

$(TARGET): boot/boot.bin default.f3s $(UTILS) $(PRGMS) kernel/kernel.bin
	$(cmd) "util/f301_builder/f301_builder default.f3s $(TARGET)" "F301_BUILDER $(TARGET)"
	$(cmd) "dd if=boot/boot.bin of=$(TARGET) conv=notrunc" "DD $(TARGET)" 2> /dev/null

boot/boot.bin: boot/*.asm
	$(cmd) "nasm -fbin -Ikernel/src/ -Iboot/ -o boot/boot.bin boot/boot.asm" "NASM boot.bin"

kernel/kernel.bin: kernel/src/*
	$(cmd) "$(MAKE) -C kernel" "MAKE kernel"

$(UTILS): $(addsuffix /src/*,$(UTILDIRS))
	$(cmd) "$(MAKE) -C $(@D)" "MAKE $(@D)"
	
$(PRGMS): $(addsuffix /src/*,$(PRGMDIRS))
	$(cmd) "$(MAKE) -C $(@D)" "MAKE $(@D)"

.PHONY: clean
clean:
	-$(cmd) "$(RM) $(TARGET)" "RM $(TARGET)"
	-$(cmd) "$(RM) boot/boot.bin" "RM boot/boot.bin"
	-$(cmd) "$(MAKE) -C kernel clean" "MAKE CLEAN kernel"
	-@for DIR in $(UTILDIRS); do $(cmdl) "$(MAKE) -C $$DIR clean" "MAKE CLEAN $$DIR"; done
	-@for DIR in $(PRGMDIRS); do $(cmdl) "$(MAKE) -C $$DIR clean" "MAKE CLEAN $$DIR"; done

