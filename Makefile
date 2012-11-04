#
# makefile for Frosk 64
#
# written by sjrct
#

SHELL      = /bin/sh

TARGET     = frosk.img

UTILS_B    = f301_builder
PRGMS_B    = start frash fui rusk
LIBS_B     = cstd fapi ykw events shiny

MAKE       := $(MAKE) --no-print-directory

CC         = gcc

export TOPDIR = $(shell pwd)

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
UTILS      = $(join $(UTILDIRS), $(addprefix /,$(UTILS_B)))

PRGMDIRS   = $(PRGMS_B:%=prgm/%)
PRGMS      = $(join $(PRGMDIRS), $(addprefix /,$(PRGMS_B)))

LIBDIRS    = $(LIBS_B:%=lib/%)
LIBS       = $(addsuffix .a,$(join $(LIBDIRS), $(addprefix /,$(LIBS_B))))
export LIBS_FULL  = $(addprefix $(TOPDIR)/,$(LIBS))

# standardize cc/ld arguments
export FROSK_CCFL = -Wall -m64 -ffreestanding -mno-red-zone -mcmodel=large \
                    -nostdinc -I$(TOPDIR)/include
export FROSK_LDFL = $(TOPDIR)/link/preamble.o -m64 -ffreestanding -mno-red-zone \
                    -mcmodel=large -nostdlib -T$(TOPDIR)/link/fbe.ld 

# dependecies and commands
.PHONY: all
all: $(TARGET)

$(TARGET): boot/boot.bin default.f3s link/preamble.o $(UTILS) $(LIBS) $(PRGMS) kernel/kernel.bin
	$(cmd) "util/f301_builder/f301_builder default.f3s $(TARGET)" "F301_BUILDER $(TARGET)"
	$(cmd) "dd if=boot/boot.bin of=$(TARGET) conv=notrunc" "DD $(TARGET)" 2> /dev/null

boot/boot.bin: boot/*.asm
	$(cmd) "nasm -fbin -Ikernel/src/ -Iboot/ -o boot/boot.bin boot/boot.asm" "NASM boot.bin"

link/preamble.o: link/preamble.asm
	$(cmd) "nasm $< -o $@ -felf64" "NASM $@"

kernel/kernel.bin: kernel/src/*
	$(cmd) "$(MAKE) -C kernel" "MAKE kernel"

$(UTILS): $(addsuffix /src/*,$(UTILDIRS))
	$(cmd) "$(MAKE) -C $(@D)" "MAKE $(@D)"
	
$(LIBS): $(addsuffix /src/*,$(LIBDIRS))
	$(cmd) "$(MAKE) -C $(@D)" "MAKE $(@D)"

$(PRGMS): $(addsuffix /src/*,$(PRGMDIRS))
	$(cmd) "$(MAKE) -C $(@D)" "MAKE $(@D)"

.PHONY: clean
clean:
	-$(cmd) "$(RM) $(TARGET)" "RM $(TARGET)"
	-$(cmd) "$(RM) boot/boot.bin" "RM boot/boot.bin"
	-$(cmd) "$(RM) link/preamble.o" "RM link/preamble.o"
	-$(cmd) "$(MAKE) -C kernel clean" "MAKE CLEAN kernel"
	-@for DIR in $(UTILDIRS); do $(cmdl) "$(MAKE) -C $$DIR clean" "MAKE CLEAN $$DIR"; done
	-@for DIR in $(LIBDIRS); do $(cmdl) "$(MAKE) -C $$DIR clean" "MAKE CLEAN $$DIR"; done
	-@for DIR in $(PRGMDIRS); do $(cmdl) "$(MAKE) -C $$DIR clean" "MAKE CLEAN $$DIR"; done

