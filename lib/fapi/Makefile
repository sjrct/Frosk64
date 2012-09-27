#
# Makefile for fapi
#
# written by sjrct
#

SHELL   = /bin/sh

TARGET  = fapi.a

CSRCS_B = stream.c
ASRCS_B = fapi.asm

CC      = gcc
AR      = ar

CSRCS = $(addprefix src/,$(CSRCS_B))
COBJS = $(addprefix cobj/,$(CSRCS_B:.c=.o))
ASRCS = $(addprefix src/,$(ASRCS_B))
AOBJS = $(addprefix aobj/,$(ASRCS_B:.asm=.o))

.PHONY: all
all: cobj aobj $(TARGET)

cobj:
	$(cmd) "mkdir cobj" "MKDIR cobj"

aobj:
	$(cmd) "mkdir aobj" "MKDIR aobj"

$(TARGET): $(COBJS) $(AOBJS)
	$(cmd) "$(AR) rcu $@ $^" "AR $(TARGET)"

cobj/%.o: src/%.c
	$(cmd) "$(CC) -c $< $(FROSK_CCFL) -o $@" "CC $@"

aobj/%.o: src/%.asm
	$(cmd) "nasm $< -felf64 -o $@" "NASM $@"

.PHONY: clean
clean:
	-$(cmd) "$(RM) $(TARGET)" "RM $(TARGET)"
	-$(cmd) "$(RM) -rf cobj aobj" "RM cobj aobj"
